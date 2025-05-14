#!/bin/bash
set -e

SCRIPT_DIR="$(dirname "$(readlink -f "${BASH_SOURCE:-$0}")")"
TOP_DIR="$(readlink -f $SCRIPT_DIR/..)"
cd $TOP_DIR

## No prefix because my default compiler is x86 
GCCPREFIX=
QEMU=qemu-system-i386

make clean
make cscope

## Run without gdb
# make qemu
## Ctrl+Alt+g to release grab

## Run without gdb, -nographic mode
# make qemu-nox
## Ctrl+a c to toggle monitor
## Ctrl+a x to terminate qemu

## For debugging with gdb
make V=1 qemu-nox-gdb
### then run make gdb or make gdb-tui in another pane

