@echo off

REM ********************************************************************
REM
REM newproject.bat
REM
REM This windows batch script generates a set of gameplay project files.
REM The new project will be based of the template project and 
REM it will be generated with the name and location that is specified
REM as input parameters.
REM
REM IMPORTANT: This script must be run from the root of the gameplay
REM source tree.
REM
REM ********************************************************************

echo.
echo 1. Enter a name for the new project.
echo.
echo    This name will be given to the project 
echo    executable and a folder with this name
echo    will be created to store all project files.
echo    Ex. foobar
echo.
set /p projName=Project name: 
if "%projName%" == "" (
    echo.
    echo ERROR: No project name specified.
    echo.
    pause
    goto done
)
echo.

echo.
echo 2. Enter a game title.
echo.
echo    On some platforms, this title is used to
echo    identify the game during installation and
echo    on shortcuts/icons.
echo    Ex. Foo Bar
echo.
set /p title=Title: 
if "%title%" == "" (
    echo.
    echo ERROR: No game title specified.
    echo.
    pause
    goto done
)
echo.

echo.
echo 3. Enter a unique identifier for your project.
echo.
echo    This should be a human readable package name,
echo    containing at least two words separated by a
echo    period.
echo    Ex. com.example.foobar
echo.
set /p uuid=Unique ID: 
if "%uuid%" == "" (
    echo.
    echo ERROR: No uuid specified.
    echo.
    pause
    goto done
)
echo.

echo.
echo 4. Enter your game's main class name.
echo.
echo    Your initial game header and source file
echo    will be given this name and a class with 
echo    this name will be created in these files.
echo    Ex. FooBarGame
echo.
set /p className=Class name: 
if "%className%" == "" (
    echo.
    echo ERROR: No class name specified.
    echo.
    pause
    goto done
)
echo.

echo.
echo 5. Enter the project path.
echo.
echo    This can be a relative path, absolute path,
echo    or empty for the current folder. Note that
echo    a project folder named %projName% will also
echo    be created inside this folder.
echo    Ex. ./samples
echo.
set /p location=Path: 
if "%location%" == "" (
    set projPath=%projName%
) else (
    set projPath=%location%\%projName%
)
echo.

call:replacevar projPath "/" "\"

REM Does this path already exist?
if exist "%projPath%" (
    echo.
    echo ERROR: Path '%projPath%' already exists, aborting.
    echo.
    pause
    goto done

REM Disabling following code which prompts to overwrite existing path,
REM since this could be potentially damaging if the user specifies
REM an important path and then types 'y', without thinking.
REM
REM    set /p owd=Directory '%projPath%' exists, overwrite [Y,N]? 
REM    if not "!owd!" == "Y" (
REM        if not "!owd!" == "y" (
REM            echo Aborting.
REM            pause
REM            goto done
REM        )
REM    )
REM    rmdir /S /Q %projPath%
)

REM Generate relative path from project folder to GamePlay folder
set gpPath=%cd%
call:makerelative gpPath "%projPath%\"

mkdir "%projPath%"
mkdir "%projPath%\src"
mkdir "%projPath%\res"

REM Copy Microsoft Visual Studio project files
copy template\template.vcxproj "%projPath%\%projName%.vcxproj"
call:replace "%projPath%\%projName%.vcxproj" TEMPLATE_PROJECT "%projName%"
call:replace "%projPath%\%projName%.vcxproj" TemplateGame "%className%"
call:replace "%projPath%\%projName%.vcxproj" GAMEPLAY_PATH "%gpPath%"
copy template\template.vcxproj.filters "%projPath%\%projName%.vcxproj.filters"
call:replace "%projPath%\%projName%.vcxproj.filters" TemplateGame "%className%"
call:replacevar gpPath "\" "/"

REM Copy Apple XCode project files
mkdir "%projPath%\%projName%.xcodeproj"
copy template\template.xcodeproj\project.pbxproj "%projPath%\%projName%.xcodeproj\project.pbxproj"
call:replace "%projPath%\%projName%.xcodeproj\project.pbxproj" GAMEPLAY_PATH "%gpPath%"
call:replace "%projPath%\%projName%.xcodeproj\project.pbxproj" TemplateGame "%className%"
call:replace "%projPath%\%projName%.xcodeproj\project.pbxproj" TEMPLATE_PROJECT "%projName%"
copy template\TEMPLATE_PROJECT-macosx.plist "%projPath%\%projName%-macosx.plist"
call:replace "%projPath%\%projName%-macosx.plist" TEMPLATE_UUID "%uuid%"
copy template\TEMPLATE_PROJECT-ios.plist "%projPath%\%projName%-ios.plist"
copy template\Default-568h@2x.png "%projPath%\Default-568h@2x.png"
call:replace "%projPath%\%projName%-ios.plist" TEMPLATE_TITLE "%title%"
call:replace "%projPath%\%projName%-ios.plist" TEMPLATE_UUID "%uuid%"

REM Copy Android NDK project files
mkdir "%projPath%\android"
copy template\android\AndroidManifest.xml "%projPath%\android\AndroidManifest.xml"
call:replace "%projPath%\android\AndroidManifest.xml" TEMPLATE_PROJECT "%projName%"
call:replace "%projPath%\android\AndroidManifest.xml" TEMPLATE_UUID "%uuid%"
copy template\android\build.xml "%projPath%\android\build.xml"
call:replace "%projPath%\android\build.xml" TEMPLATE_PROJECT "%projName%"
call:replace "%projPath%\android\build.xml" GAMEPLAY_PATH "%gpPath%"
copy template\android\project.properties "%projPath%\android\project.properties"
mkdir "%projPath%\android\jni"
copy template\android\jni\Application.mk "%projPath%\android\jni\Application.mk"
copy template\android\jni\Android.mk "%projPath%\android\jni\Android.mk"
call:replace "%projPath%\android\jni\Android.mk" TemplateGame "%className%"
call:replace "%projPath%\android\jni\Android.mk" TEMPLATE_PROJECT "%projName%"
call:replace "%projPath%\android\jni\Android.mk" GAMEPLAY_PATH "%gpPath%"
mkdir "%projPath%\android\res\drawable"
copy template\icon.png "%projPath%\android\res\drawable\icon.png"
mkdir "%projPath%\android\res\values"
copy template\android\res\values\template.strings.xml "%projPath%\android\res\values\strings.xml"
call:replace "%projPath%\android\res\values\strings.xml" TEMPLATE_TITLE "%title%"

REM Copy Eclipse files for Android
copy template\android\.cproject "%projPath%\android\.cproject"
call:replace "%projPath%\android\.cproject" TEMPLATE_PROJECT "%projName%"
call:replace "%projPath%\android\.cproject" TEMPLATE_UUID "%uuid%"
call:replace "%projPath%\android\.cproject" GAMEPLAY_PATH "%gpPath%"
copy template\android\.project "%projPath%\android\.project"
call:replace "%projPath%\android\.project" TEMPLATE_PROJECT "%projName%"
copy template\android\.classpath "%projPath%\android\.classpath"
call:replace "%projPath%\android\.classpath" TEMPLATE_PROJECT "%projName%"

REM Copy Eclipse files for Linux
copy template\.cproject "%projPath%\.cproject"
call:replace "%projPath%\.cproject" TEMPLATE_PROJECT "%projName%"
call:replace "%projPath%\.cproject" TEMPLATE_UUID "%uuid%"
call:replace "%projPath%\.cproject" GAMEPLAY_PATH "%gpPath%"
copy template\.project "%projPath%\.project"
call:replace "%projPath%\.project" TEMPLATE_PROJECT "%projName%"

REM Copy QtCreator files
copy template\TEMPLATE_PROJECT.pro "%projPath%\%projName%.pro"
call:replace "%projPath%\%projName%.pro" TEMPLATE_PROJECT "%projName%"
call:replace "%projPath%\%projName%.pro" GAMEPLAY_PATH "%gpPath%"
call:replace "%projPath%\%projName%.pro" TemplateGame %className%

REM Copy CMake files
mkdir "%projPath%\build"
copy "template\template-CMakeLists.txt" "%projPath%\CMakeLists.txt"
call:replace "%projPath%\CMakeLists.txt" TEMPLATE_PROJECT %projName%
call:replace "%projPath%\CMakeLists.txt" TemplateGame %className%
call:replace "%projPath%\CMakeLists.txt" GAMEPLAY_PATH %gpPath%

REM Copy source files
copy template\src\TemplateGame.h "%projPath%\src\%className%.h"
copy template\src\TemplateGame.cpp "%projPath%\src\%className%.cpp"
call:replace "%projPath%\src\%className%.h" TemplateGame "%className%"
call:replace "%projPath%\src\%className%.cpp" TemplateGame "%className%"

REM Copy resource files
copy template\res\* "%projPath%\res\"

REM Copy icon
copy template\icon.png "%projPath%\icon.png"

REM Copy config
copy template\game.config "%projPath%\game.config"
call:replace "%projPath%\game.config" TEMPLATE_TITLE "%title%"

REM Open new project folder
start "" "%projPath%"

goto done

:replace
set rtemp=%~1.rtemp
if exist "%rtemp%" del /Q "%rtemp%"
for /f "tokens=1* eol=€ delims=€]" %%j in ('type "%~1" ^| %windir%\system32\find /V /N ""') do (
    set line=%%k
    setlocal EnableDelayedExpansion
    if "!line!" == "" (
        echo.>>"%rtemp%"
    ) else (
        set linput=!line!
        set loutput=!linput:%~2=%~3!
        echo.!loutput!>>"%rtemp%"
    )
    endlocal
)
copy /Y "%rtemp%" "%~1"
del /Q "%rtemp%"
exit /b
goto done

:replacevar
setlocal EnableDelayedExpansion
echo !%~1!>.replacevar.tmp
endlocal
call:replace .replacevar.tmp "%~2" "%~3"
set /p replaced=<.replacevar.tmp
set %~1=%replaced%
del /Q .replacevar.tmp
exit /b
goto done

:makerelative
setlocal EnableDelayedExpansion
set src=%~1
if defined %1 set src=!%~1!
set bas=%~2
if not defined bas set bas=%cd%
for /f "tokens=*" %%a in ("%src%") do set src=%%~fa
for /f "tokens=*" %%a in ("%bas%") do set bas=%%~fa
set mat=&rem variable to store matching part of the name
set upp=&rem variable to reference a parent
for /f "tokens=*" %%a in ('echo.%bas:\=^&echo.%') do (
    set sub=!sub!%%a\
    call set tmp=%%src:!sub!=%%
    if "!tmp!" NEQ "!src!" (set mat=!sub!)ELSE (set upp=!upp!..\)
)
set src=%upp%!src:%mat%=!
( endlocal & REM RETURN VALUES
    IF defined %1 (SET %~1=%src%) ELSE ECHO.%src%
)
exit /b
goto done

:done
