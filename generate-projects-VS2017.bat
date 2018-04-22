@echo off

setlocal

if not exist "build2017" mkdir "build2017"

pushd "build2017"
    echo Generating Visual Studio 2017 projects...
    cmake -G "Visual Studio 15 2017 Win64" ..
popd
