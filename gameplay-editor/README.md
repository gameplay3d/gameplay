## GamePlay Editor

Game editor for GamePlay.

## Developer Setup

-  Windows (64-bit)
  * Install Windows 10
  * Install Visual Studio 2015
  * Intall Qt 5.8
  
- Linux (64-bit)
  * Install Ubuntu 16.04
  * Intall Qt 5.8

- MacOS (64-bit)
  * Install MacOS Sierra
  * Install XCode
  * Intall Qt 5.8


## Qt Creator
- Open Qt Creator and open the following projects:
	*  GamePlay/gameplay/gameplay.pro
	*  GamePlay/gameplay-editor/gameplay-editor.pro
- Configure output paths to "build/Debug", "build/Release", "build/Profile" on both projects
- Build project gameplay
- Build project gameplay-editor
- Set Working directory to gameplay-editor folder.
- Run gameplay-editor

## Visual Studio 2015 + Qt5 plugin
- Open Visual Studio 2015
- Open menu Tools->Extensions and Updates...
- Search for Qt5Package and install extension.
- Restart Visual Studio 2015
- Open menu QT5->Qt Options
- Add Name=Qt5.8  Path=C:\Qt\Qt5.8.0\5.8\msvc2015_64
- Open GamePlay\gameplay.sln
- Build project gameplay-editor (which depends on gameplay)
- Set gameplay-editor as active project
- Run project gameplay-editor
