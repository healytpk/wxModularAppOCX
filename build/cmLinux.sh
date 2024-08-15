#!/bin/bash
echo OS Type: $OSTYPE

# ----------------------------------
# build Debug configuration makefile
# ----------------------------------
echo building Debug configuration makefile
echo directory "LinuxDebug"
rm -dr "LinuxDebug"
mkdir "LinuxDebug"
cd "LinuxDebug"
cmake -DCMAKE_BUILD_TYPE:STRING=Debug ../ -DCMAKE_CXX_STANDARD_LIBRARIES="-lwx_gtk3ud_aui-`wx-config --version | cut -d '.' -f 1,2`" -DCMAKE_LDFLAGS="-Wl,-rpath,'$ORIGIN'"
cd ..

# ----------------------------------
# build Release configuration makefile
# ----------------------------------
echo building Release configuration makefile
echo directory "LinuxRelease"
rm -dr "LinuxRelease"
mkdir "LinuxRelease"
cd "LinuxRelease"
cmake -DCMAKE_BUILD_TYPE:STRING=Release ../ -DCMAKE_CXX_STANDARD_LIBRARIES="-lwx_gtk3ud_aui-`wx-config --version | cut -d '.' -f 1,2`" -DCMAKE_LDFLAGS="-Wl,-rpath,'$ORIGIN'"
cd ..
