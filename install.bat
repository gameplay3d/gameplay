@echo off
REM
REM gameplay-install.bat [master | next]
REM
REM Download GamePlay resources from HTTP server and extract from ZIP
REM
REM Helps prevent repo bloat due to large binary files since they can
REM be hosted separately.
REM

REM Production URL
set prefix=http://www.gameplay3d.org

set branchname=%1
if "%branchname%" == "" (
  set branchname=next
)

set filename=gameplay-deps-%branchname%

echo Downloading %filename%.zip from %prefix%
%~d0
cd %~dp0
> temp1.vbs ECHO Dim strFileURL, strHDLocation
>> temp1.vbs ECHO strFileURL = WScript.Arguments(0)
>> temp1.vbs ECHO strHDLocation = WScript.Arguments(1)
>> temp1.vbs ECHO Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP")
>> temp1.vbs ECHO objXMLHTTP.open "GET", strFileURL, false
>> temp1.vbs ECHO objXMLHTTP.send()
>> temp1.vbs ECHO If objXMLHTTP.Status = 200 Then
>> temp1.vbs ECHO Set objADOStream = CreateObject("ADODB.Stream")
>> temp1.vbs ECHO objADOStream.Open
>> temp1.vbs ECHO objADOStream.Type = 1
>> temp1.vbs ECHO objADOStream.Write objXMLHTTP.ResponseBody
>> temp1.vbs ECHO objADOStream.Position = 0
>> temp1.vbs ECHO Set objFSO = Createobject("Scripting.FileSystemObject")
>> temp1.vbs ECHO If objFSO.Fileexists(strHDLocation) Then objFSO.DeleteFile strHDLocation
>> temp1.vbs ECHO Set objFSO = Nothing
>> temp1.vbs ECHO objADOStream.SaveToFile strHDLocation
>> temp1.vbs ECHO objADOStream.Close
>> temp1.vbs ECHO Set objADOStream = Nothing
>> temp1.vbs ECHO End if
>> temp1.vbs ECHO Set objXMLHTTP = Nothing
cscript temp1.vbs %prefix%/%filename%.zip %filename%.zip

echo Extracting %filename%.zip... please standby...
%~d0
cd %~dp0
> temp2.vbs ECHO Dim fileName, workingDir
>> temp2.vbs ECHO fileName = WScript.Arguments(0)
>> temp2.vbs ECHO workingDir = CreateObject("Scripting.FileSystemObject").GetAbsolutePathName(".")
>> temp2.vbs ECHO Set objShell = CreateObject("Shell.Application")
>> temp2.vbs ECHO Set objSource = objShell.NameSpace(workingDir ^& "\" ^& fileName).Items()
>> temp2.vbs ECHO Set objTarget = objShell.NameSpace(workingDir ^& "\")
>> temp2.vbs ECHO intOptions = 256
>> temp2.vbs ECHO objTarget.CopyHere objSource, intOptions
cscript temp2.vbs %filename%.zip
echo Cleaning up...
del temp1.vbs
del temp2.vbs
del %filename%.zip
echo Done.
