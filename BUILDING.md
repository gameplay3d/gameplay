Windows 10
----------
- Install Windows 10 SDK (10.0.15063.xxx)    (https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- Install Visual Studio 2015 + Update 3      (https://www.visualstudio.com/vs/older-downloads/)
- Install Qt 5.9  (Ex. C:\Qt)                (https://www1.qt.io/download-open-source/)
- Set QTDIR environment variable             (QTDIR=C:\Qt\5.9.1\msvc2015_64)
- Install Qt extension in Visual Studio 2015 (Tools->Extension and Updates->Online->Search for "Qt Visual Studio Tools")
- Setup Qt extension in Visual Studio 2015   (Qt VS Tools->Qt Options->Add->Path=C:\Qt\5.9.1\msvc2015_64)
- Install Vulkan SDK                         (https://vulkan.lunarg.com/sdk/home#windows)
- Set VULKAN_SDK environment variable        (VULKAN_SDK=C:\VulkanSDK\1.0.61.1)
- Run GamePlay\install.bat
- Run Visual Studio 2015
- Open GamePlay\GamePlay.sln
- Set gameplay-editor as active project
- Build and Run

Ubuntu 16.04 LTS (x86_64)
-------------------------
- Install SDKs                              (sudo apt-get install build-essential gcc g++ curl libglu1-mesa-dev libx11-xcb-dev libgtk2.0-dev libogg-dev libopenal-dev)
- Install Qt 5.9 (Ex. ~/Qt)                 (https://www1.qt.io/download-open-source/)
- Install Vulkan SDK                        (https://vulkan.lunarg.com/sdk/home#linux)
- Add the following to user profile         (vi ~/.profile)
```
export VULKAN_SDK=~/VulkanSDK/1.0.61.1/x86_64 
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
export VK_LAYER_PATH=$VULKAN_SDK/etc/explicit_layer.d
```
- Reboot computer
- Run GamePlay/install.sh
- Run Qt Creator
- Open GamePlay/gameplay/gameplay.pro
- Open GamePlay/gameplay/gamplay-app.pro 
- Open GamePlay/gameplay-editor/gameplay-editor.pro
- For each project "Configure Project" kits by expanding "Desktop Qt 5.9.1 GCC 64bit"
- Change the long default build paths for Debug and Release to:
```
Debug = "Debug"
Release = "Release"
```
- Build and Run

MacOS High Sierra 
-----------------
- Install Xcode 9                           (https://developer.apple.com/xcode/)
- Install Qt 5.9 (Ex. ~/Qt)                 (https://www1.qt.io/download-open-source/)
- Run GamePlay/install.sh
- Open GamePlay/gameplay/gameplay.pro
- Open GamePlay/gameplay/gamplay-app.pro 
- Open GamePlay/gameplay-editor/gameplay-editor.pro
- For each project "Configure Project" kits by expanding "Desktop Qt 5.9.1 Clang 64bit"
- Change the long default build paths for Debug and Release to:
```
Debug = "Debug"
Release = "Release"
```
- Build and Run