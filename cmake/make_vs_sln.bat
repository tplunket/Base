@echo off
setlocal
cd /d %~dp0

pushd ..
set SRC_DIR="%CD%"
popd

call :Create win32 "Visual Studio 15 2017"
call :Create win64 "Visual Studio 15 2017 Win64"
goto :eof

:Create
    set OUTDIR="%~1"
    set TARGET="%~2"
    if not exist %OUTDIR% mkdir %OUTDIR%
    pushd %OUTDIR%
    cmake -G %TARGET% %SRC_DIR%
    popd
    goto :eof
