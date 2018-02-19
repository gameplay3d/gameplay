@echo off

setlocal

if not exist "build" mkdir "build"

pushd "build"
    echo Generating Visual Studio projects...
    cmake -G "Visual Studio 14 2015 Win64" ..
popd
