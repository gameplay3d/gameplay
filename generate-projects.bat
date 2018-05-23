@echo off

setlocal

if not exist "build" mkdir "build"

pushd "build"
    echo Generating Visual Studio projects...
    cmake -G "Visual Studio 15 2017 Win64" ..
popd
