@echo off
set VSCMD_START_DIR=.
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
lib /machine:"amd64" %*
