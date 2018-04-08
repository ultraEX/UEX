UEX version 1.0.1.0 is now available from:

  <https://github.com/ultraEX/UEX/bin/bitcoin-core-*version*/>

Wallet changes
---------------
This is a new major version release, including new features:
1. no mine; 
2. no POW; 
3. energy-conservation; 
4. decentralization!

Please report bugs using the issue tracker at GitHub:

  <https://github.com/ultraEX/UEX/issues>

To receive security and update notifications, please subscribe to:

  <https://github.com/ultraEX/UEX/en/list/announcements/join/>

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions), then run the
installer (on Windows) or just copy over `/Applications/uex-Qt` (on Mac)
or `uexd`/`uex-qt` (on Linux).


Compatibility
==============

UEX is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows 7 and newer (Windows XP is not supported).

UEX should also work on most other Unix-like systems but is not
frequently tested on them.

Notable changes
===============

RPC changes
------------

### Low-level changes

- The `fundrawtransaction` rpc will reject the previously deprecated `reserveChangeKey` option.

Credits
=======

Thanks to everyone who directly contributed to this release:


As well as everyone that helped translating on [Transifex](https://www.transifex.com/projects/p/UEX/).
