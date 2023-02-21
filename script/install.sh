#!/bin/bash
set -e

SCRIPT_DIR="$(dirname "$(readlink -f "${BASH_SOURCE:-$0}")")"
TOP_DIR="$(readlink -f $SCRIPT_DIR/..)"
cd $TOP_DIR

# 6.828-qemu build and installation
if [ ! -d 6.828-qemu ]; then
	git clone https://github.com/mit-pdos/6.828-qemu.git 6.828-qemu
else
	echo "Looks like qemu repo has already been cloned, skipping git clone..."
fi

if [ ! -f /usr/include/glib.h ]; then
	sudo ln -s /usr/include/glib-2.0/glib.h /usr/include/glib.h
fi

if [ ! -d /usr/include/glib ]; then
	sudo ln -s /usr/include/glib-2.0/glib /usr/include/glib
fi

if [ ! -f /usr/include/glibconfig.h ]; then
	sudo ln -s /usr/lib/x86_64-linux-gnu/glib-2.0/include/glibconfig.h /usr/include/glibconfig.h
fi

cd 6.828-qemu
# My machine is x86, so I'll set proper CC and unset CROSS_COMPILE to avoid accidentally invoking my cross-compiler
export CC=/usr/bin/gcc
export CROSS_COMPILE=
./configure --disable-kvm --disable-werror --prefix=/usr/local --target-list="i386-softmmu x86_64-softmmu" --python=/usr/bin/python2
make -j$(nproc) CFLAGS="-DXC_WANT_COMPAT_EVTCHN_API -DXC_WANT_COMPAT_GNTTAB_API"
sudo make install

