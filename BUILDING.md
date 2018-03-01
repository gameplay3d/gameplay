Windows 10
----------
- Install CMake 3                            (https://cmake.org/download/)
- Install Windows 10 SDK (10.0.15063.xxx)    (https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- Install Visual Studio 2015 + Update 3      (https://www.visualstudio.com/vs/older-downloads/)
- Install Vulkan SDK                         (https://vulkan.lunarg.com/sdk/home#windows)
- Install Qt 5.10 (open-source) to C:\Qt)    (https://www.qt.io/download)
- Set QTDIR environment variable             (QTDIR=C:\Qt\5.10.1\msvc2015_64)
- Run install.bat
- Run Qt Creator
- Open gameplay/gameplay.pro
- Open gameplay/gameplay-editor.pro
- Expand "Configure Project" kits by expanding "Desktop Qt 5.10.1 MSVC2015 64bit"
- Change long build paths for Debug and Release to:
```
Debug = "Debug"
Release = "Release"
Profile = "Profile"
```
- Build and Run

Ubuntu 16.04
------------
- Install CMake 3                           (sudo apt-get install cmake)
- Install Build SDKs                        (sudo apt-get install build-essential clang gcc g++ curl)
- Install Platform SDKs                     (sudo apt-get install libx11-xcb-dev libgtk2.0-dev libogg-dev libopenal-dev)
- Install Vulkan SDK                        (https://vulkan.lunarg.com/sdk/home#linux)
- Install Qt 5.10 (open-source) to ~/Qt     (https://www1.qt.io/download)
- Run install.sh
- Run Qt Creator
- Open gameplay/gameplay.pro
- Open gameplay-editor/gameplay-editor.pro
- Expand "Configure Project" kits by expanding "Desktop Qt 5.10.1 GCC 64bit"
- Change long build paths for Debug and Release to:
```
Debug = "Debug"
Release = "Release"
Profile = "Profile"
```
- Build and Run

MacOS High Sierra
-----------------
- Install CMake 3                           (https://cmake.org/download/)
- Install Xcode 9                           (https://developer.apple.com/xcode/)
- Install Xcode Command Line Tools          (https://developer.apple.com/download/more/)
- Install Qt 5.10 (open-source) to ~/Qt     (https://www1.qt.io/download)
- Run install.sh
- Open gameplay/gameplay.pro
- Open gameplay-editor/gameplay-editor.pro
- Expand "Configure Project" kits by expanding "Desktop Qt 5.10.1 Clang 64bit"
- Change long build paths for Debug and Release to:
```
Debug = "Debug"
Release = "Release"
Profile = "Profile"
```
- Build and Run
