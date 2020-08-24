#!/bin/bash

VERSION="2.34"

mkdir -p ./dep-build/binutils
pushd ./dep-build/binutils
wget "https://mirror.csclub.uwaterloo.ca/gnu/binutils/binutils-$VERSION.tar.xz"
tar -xf "binutils-$VERSION.tar.xz"
"binutils-$VERSION/configure" --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
make -j $THREADS
make install
popd
