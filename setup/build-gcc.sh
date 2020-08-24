#!/bin/bash

which -- $TARGET-as || echo "$TARGET-as is not in PATH"

VERSION="9.3.0"

mkdir -p ./dep-build/gcc
pushd ./dep-build/gcc
wget "https://mirror.csclub.uwaterloo.ca/gnu/gcc/gcc-$VERSION/gcc-$VERSION.tar.xz"
tar -xf "gcc-$VERSION.tar.xz"
"gcc-$VERSION/configure" --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j $THREADS
make all-target-libgcc -j $THREADS
make install-gcc
make install-target-libgcc
popd
