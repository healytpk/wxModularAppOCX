#!/bin/bash

rm -dr Mac
mkdir Mac
cd Mac
cmake ../ -DCMAKE_CXX_STANDARD_LIBRARIES="-lwx_gtk3ud_aui-3.2"
cd ..
