Windows 10 (x86_64)
-------------------
- Install Windows 10 SDK (10.0.15063.xxx) 	(https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- Install Visual Studio 2015 + Update 3  	(https://www.visualstudio.com/vs/older-downloads/)
- Install Qt 5.9+  (Ex. C:\Qt)   		(https://www1.qt.io/download-open-source/)
- Set QTDIR environment variable 		(QTDIR=C:\Qt\5.9.1\msvc2015_64)
- Install Qt extension in Visual Studio 2015 	(Goto Tools->Extension and Updates->Online->Search for "Qt Visual Studio Tools")
- Setup Qt extension in Visual Studio 2015 	(Goto Qt VS Tools->Qt Options->Add->Path=C:\Qt\5.9.1\msvc2015_64)
- Install Vulkan SDK 				(https://vulkan.lunarg.com/sdk/home#windows)
- Set VULKAN_SDK environment variable		(VULKAN_SDK=C:\VulkanSDK\1.0.61.1)
- Run GamePlay\install.bat
- Open GamePlay\GamePlay.sln
- Set gameplay-editor as active project
- Build and Run

Ubuntu 16.04 LTS (x86_64)
-------------------------
- Install POSIX SDK tools			(sudo apt-get install build-essential gcc g++ curl libogg-dev libopenal-dev libx11-xcb-dev)
- Install Qt 5.9+ (Ex. ~/Qt) 			(https://www1.qt.io/download-open-source/)
- Set QTDIR environment variable 		(Ex. QTDIR=~\Qt\5.9.1\msvc2015_64)
- Install Vulkan SDK				(https://vulkan.lunarg.com/sdk/home#linux)
- Add the following to system profle 		(sudo vi /etc/profile)
```
export VULKAN_SDK=~/VulkanSDK/1.0.61.1/x86_64 
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
export VK_LAYER_PATH=$VULKAN_SDK/etc/explicit_layer.d
```
- Run GamePlay/install.sh
- Open Qt Creator projects:
	GamePlay/gameplay/gameplay.pro
	GamePlay/gameplay/gamplay-app.pro 
	GamePlay/gameplay-editor/gameplay-editor.pro
- Build and Run

MacOS High Sierra (x86_64)
--------------------------
- TODO
