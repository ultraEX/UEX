UEX integration/staging tree
=====================================

[![Build Status](https://travis-ci.org/ultraEX/UEX.svg?branch=master)](https://travis-ci.org/ultraEX/UEX)

https://ultraEX.io

What is UEX?
----------------

UEX is the world's first coin of the trading platform, issued by the 
ultraEX.io exchange based on blockchain. UEX can be used and circulated under 
many Eco scenarios, including blockchain and ultraNews.io agency of financial 
media platforms, mining pools ultraPool, exchanges ultraEX.io, as well as sign 
the contract with U-PP,an excellent blockchain skill sharing service platforms
with over 2 million all over the world to use UEX as payment gateway. Users who
hold UEX will also enjoy special discounts on transaction fees for ultraEX.io 
exchanges. In addition, UEX will also act as a reward for circulation of 
"Beauty Talking Cryptourrency" for the online financial live channel, and users
can use UEX as a reward to them. 


For more information, as well as an immediately useable, binary version of
the UEX software, see https://github.com/ultraEX/UEX, or read the 
[bitcoin original whitepaper](https://bitcoincore.org/bitcoin.pdf).

License
-------

UEX is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/ultraEX/UEX/tags) are created
regularly to indicate new official, stable release versions of UEX.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

The developer [mailing list](https://lists.linuxfoundation.org/mailman/listinfo/UEX-dev)
should be used to discuss complicated or controversial changes before working
on a patch set.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

