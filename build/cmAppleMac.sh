#!/bin/bash

rm -dr Mac
mkdir Mac
cd Mac
cmake ../ -DCMAKE_CXX_STANDARD_LIBRARIES="-l:libwx_osx_cocoau_aui-3.2.dylib"
cd ..
