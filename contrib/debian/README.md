
Debian
====================
This directory contains files used to package uexd/uex-qt
for Debian-based Linux systems. If you compile uexd/uex-qt yourself, there are some useful files here.

## bitcoin: URI support ##


uex-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install uex-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your uex-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcoin128.png` to `/usr/share/pixmaps`

uex-qt.protocol (KDE)

