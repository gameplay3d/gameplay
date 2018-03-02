Windows 10
----------
- Install CMake 3                            (https://cmake.org/download/)
- Install Windows 10 SDK (10.0.15063.xxx)    (https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- Install Visual Studio 2015 + Update 3      (https://www.visualstudio.com/vs/older-downloads/)
- Install Vulkan SDK                         (https://vulkan.lunarg.com/sdk/home#windows)
- Install Qt 5.10 (open-source) to C:\Qt)    (https://www.qt.io/download)
- Clone GamePlay repo
- Run install.bat
- Set QTDIR environment variable             (QTDIR=C:\Qt\5.10.1\msvc2015_64)
- Run Qt Creator
- Open gameplay/gameplay.pro
- Open gameplay-app/gameplay-app.pro
- Open gameplay-editor/gameplay-editor.pro
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
- Install CMake                             (sudo apt-get install cmake)
- Install Build SDKs                        (sudo apt-get install build-essential clang gcc g++ curl)
- Install Platform SDKs                     (sudo apt-get install libx11-xcb-dev libgtk2.0-dev libogg-dev libopenal-dev 
libtool libasound2-dev  libpulse-dev libaudio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libxss-dev libgl1-mesa-dev libesd0-dev)
- Install Vulkan SDK                        (https://vulkan.lunarg.com/sdk/home#linux)
- Add the following to user profile         (vi ~/.profile)
```
export VULKAN_SDK=~/VulkanSDK/1.0.68.0/x86_64 
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
```
- Reboot computer
- Install Qt 5.10 (open-source) to ~/Qt     (https://www1.qt.io/download)
- Clone GamePlay repo
- Run install.sh
- Run Qt Creator
- Open gameplay/gameplay.pro
- Open gameplay-app/gameplay-app.pro
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
- Install CMake                             (https://cmake.org/download/)
- Install Xcode                             (https://developer.apple.com/xcode/)
- Install Xcode Command Line Tools          (https://developer.apple.com/download/more/)
- Install Vulkan SDK                        (https://vulkan.lunarg.com/sdk/home#macos)
- Extract vulkansdk-macos-1.0.69.0.tar.gz to ~/vulkansdk-macos-1.0.69.0
- Add the following to user profile         (vi ~/.profile)
```
export VULKAN_SDK="~/vulkansdk-macos-1.0.69.0/macOS"
export VK_ICD_FILENAMES=$VULKAN_SDK/etc/vulkan/icd.d/MoltenVK_icd.json
```
- Reboot computer
- Install Qt 5.10 (open-source) to ~/Qt     (https://www1.qt.io/download)
- Clone GamePlay repo
- Run install.sh
- Open gameplay/gameplay.pro
- Open gameplay-app/gameplay-app.pro
- Open gameplay-editor/gameplay-editor.pro
- Expand "Configure Project" kits by expanding "Desktop Qt 5.10.1 clang 64bit"
- Change long build paths for Debug and Release to:
```
Debug = "Debug"
Release = "Release"
Profile = "Profile"
```
- Build and Run
