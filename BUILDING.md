Windows 10
----------
- Install CMake 3.2+                         (https://cmake.org/download/)
- Install Windows 10 SDK (10.0.15063.xxx)    (https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- Install Visual Studio 2015 + Update 3      (https://www.visualstudio.com/vs/older-downloads/)
- Install Vulkan SDK                         (https://vulkan.lunarg.com/sdk/home#windows)
- Run GamePlay\install.bat
- Run GamePlay\generate-projects.bat
- Open GamePlay\build\GamePlay.sln
- Set gameplay-editor as active project
- Build and Run

Ubuntu 16.04
------------
- Install CMake 3.2+                        (sudo apt-get install cmake)
- Install Build SDKs                        (sudo apt-get install build-essential clang gcc g++)
- Install Platform SDKs                     (sudo ap-get install libx11-xcb-dev libgtk2.0-dev libogg-dev libopenal-dev)
- Install Vulkan SDK                        (https://vulkan.lunarg.com/sdk/home#linux)
- Run GamePlay/install.sh
- Run GamePlay/generate-projects.sh
- cd build
- make
- ./tools/editor/gameplay-editor

MacOS High Sierra
-----------------
- Install CMake 3.2+                        (https://cmake.org/download/)
- Install Xcode 9                           (https://developer.apple.com/xcode/)
- Install Xcode Command Line Tools          (https://developer.apple.com/download/more/)
- Run GamePlay/install.sh
- Run GamePlay/generate-projects.sh
- Open GamePlay/build/GamePlay.xcodeproj
- Build gameplay and gameplay-editor
- Run gameplay-editor
