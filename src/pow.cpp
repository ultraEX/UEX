// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>


/**
 * Calc adjust window average work difficulty:
 */
arith_uint256 CalcWindowWorkAvg(const CBlockIndex* pindexLast, const Consensus::Params& params)
{
    static arith_uint256 bnAvg = UintToArith256(params.powLimit);
    static int nHeight = 0xffffffff;
    if (pindexLast->nHeight == nHeight)
        return bnAvg;

    const CBlockIndex* pindexFirst = pindexLast;
    arith_uint256 bnTot {0};
    int64_t iAdjustWindow = params.DifficultyAdjustmentInterval();
    for (int i = 0; pindexFirst && i < iAdjustWindow; i++) {
        arith_uint256 bnTmp;
        bnTmp.SetCompact(pindexFirst->nBits);
        bnTot += bnTmp;
        pindexFirst = pindexFirst->pprev;
    }
    
    if (pindexFirst == NULL)
        return UintToArith256(params.powLimit);
    
    bnAvg = bnTot / iAdjustWindow;
    nHeight = pindexLast->nHeight;
    LogPrint(BCLog::BENCH, "%s: iAdjustWindow=%i, bnAvg=%s\n", __func__, iAdjustWindow, bnAvg.GetHex());
    return bnAvg;
}

/**
 * Difficulty get from actual block timespan:
 */
unsigned int CalculateWorkRequiredFromTimeSpan(const CBlockIndex* pindexLast, int64_t nActualTimespan, const Consensus::Params& params)
{
    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew {CalcWindowWorkAvg(pindexLast, params)};
    //bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    unsigned int nNewBn = bnNew.GetCompact();
    LogPrint(BCLog::BENCH, "%s: nActualTimespan=%i, nNewBn=%d, bnNew=%s\n", __func__, nActualTimespan, nNewBn, bnNew.GetHex());
    return nNewBn;
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan / 4)
        nActualTimespan = params.nPowTargetTimespan / 4;
    if (nActualTimespan > params.nPowTargetTimespan * 4)
        nActualTimespan = params.nPowTargetTimespan * 4;

    // Retarget
    return CalculateWorkRequiredFromTimeSpan(pindexLast, nActualTimespan, params);
}

/**
 * cliffs protected:
 * If mining block spent every more than 1h,  reduce a fix difficulty value by 25%. equivalently: nActualTimespan/3 as timespan input to CalculateWorkRequiredFromTimeSpan
 */
bool CliffsProtect(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params, unsigned int& nNewBn)
{
    // If the mining block have spent more than 1h (1h=3600s): equivalently params.nPowTargetSpacing * 12
    if (pblock->GetBlockTime() < pindexLast->GetBlockTime())
        return false;

    int64_t nActualTimespan = 0;
    nActualTimespan = pblock->GetBlockTime() - pindexLast->GetBlockTime();
    if (nActualTimespan > 3600) {
        nNewBn = CalculateWorkRequiredFromTimeSpan(pindexLast, nActualTimespan, params);
        LogPrint(BCLog::BENCH, "%s: nActualTimespan=%i, nNewBn=%d\n", __func__, nActualTimespan, nNewBn);
        return true;
    } else
        return false;
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();
    unsigned int nNewBn = pindexLast->nBits;

    /// when the chain start need to initialize:
    if (pindexLast->nHeight < params.DifficultyAdjustmentInterval()){
        return pindexLast->nBits;
    }

    // adjust every block
    if (params.fPowAllowMinDifficultyBlocks) {
        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2* 10 minutes
        // then allow mining of a min-difficulty block.
        if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 2)
            return nProofOfWorkLimit;
        else {
            // Return the last non-special-min-difficulty-rules-block
            const CBlockIndex* pindex = pindexLast;
            while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                pindex = pindex->pprev;
            return pindex->nBits;
        }
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval() - 1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    // Retarget
    nNewBn = CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);

    // Cliff Protect
    CliffsProtect(pindexLast, pblock, params, nNewBn);

    return nNewBn;
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
