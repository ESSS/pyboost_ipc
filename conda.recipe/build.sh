#!/bin/bash

mkdir build
cd build

cmake \
    -DCMAKE_INSTALL_PREFIX:PATH="$PREFIX" \
    -DPYTHON_EXECUTABLE:FILEPATH=$PYTHON \
    -DPYTHON_INCLUDE_PATH:PATH=$PREFIX/include/python2.7 \
    -DPYTHON_LIBRARY:FILEPATH=$PREFIX/lib/$PY_LIB \
    ..

make -j${CPU_COUNT}
make install
