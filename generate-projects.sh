#!/bin/bash

if [ ! -d "build" ]; then mkdir -p "build"; fi

pushd "build"
    if [ "$(uname)" == "Darwin" ]; then
        echo Generating xcode projects...
        cmake -G "Xcode" ..
    elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
         echo Generating linux makefiles...
        cmake -G "Unix Makefiles" ..
    else
    echo Platform not supported.
    fi
popd
