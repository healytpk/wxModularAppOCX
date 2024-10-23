rem @echo off
rmdir /S /Q Win
mkdir Win
cd Win
cmake ../ -G "Visual Studio 17 2022"
copy ..\DllExport.bat .
REM --- Here we somehow run DllExport ---
cd ..
