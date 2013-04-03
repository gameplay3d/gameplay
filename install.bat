@echo off
REM
REM install.bat [master | next]
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
> temp.cs ECHO using System;
>> temp.cs ECHO using System.Net;
>> temp.cs ECHO using System.ComponentModel;
>> temp.cs ECHO class Program
>> temp.cs ECHO {
>> temp.cs ECHO     static string file = "%filename%.zip";
>> temp.cs ECHO     static string url = "%prefix%/" + file;
>> temp.cs ECHO     static bool done = false;
>> temp.cs ECHO     static void Main(string[] args)
>> temp.cs ECHO     {
>> temp.cs ECHO         try
>> temp.cs ECHO         {
>> temp.cs ECHO             WebClient client = new WebClient();
>> temp.cs ECHO             client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(DownloadProgressChanged);
>> temp.cs ECHO             client.DownloadFileCompleted += new AsyncCompletedEventHandler(DownloadFileCompleted);
>> temp.cs ECHO             Console.Write("Downloading " + url + ": 0%%    ");
>> temp.cs ECHO             client.DownloadFileAsync(new Uri(url), file);
>> temp.cs ECHO             while (!done) System.Threading.Thread.Sleep(1000);
>> temp.cs ECHO         }
>> temp.cs ECHO         catch (Exception x)
>> temp.cs ECHO         {
>> temp.cs ECHO             Console.WriteLine("Error: " + x.Message);
>> temp.cs ECHO         }
>> temp.cs ECHO     }
>> temp.cs ECHO     static void DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
>> temp.cs ECHO     {
>> temp.cs ECHO         Console.Write("\rDownloading " + url + ": " + e.ProgressPercentage + "%%    ");
>> temp.cs ECHO     }
>> temp.cs ECHO     static void DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
>> temp.cs ECHO     {
>> temp.cs ECHO         Console.WriteLine("\rDownloading " + url + ": Done.    ");
>> temp.cs ECHO         done = true;
>> temp.cs ECHO     }
>> temp.cs ECHO }
> temp1.vbs ECHO WScript.Echo "Downloading using a fallback method. This might take a few minutes."
>> temp1.vbs ECHO Dim strFileURL, strHDLocation
>> temp1.vbs ECHO strFileURL = WScript.Arguments(0)
>> temp1.vbs ECHO strHDLocation = WScript.Arguments(1)
>> temp1.vbs ECHO Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP")
>> temp1.vbs ECHO objXMLHTTP.open "GET", strFileURL, false
>> temp1.vbs ECHO WScript.Echo "Sending request..."
>> temp1.vbs ECHO objXMLHTTP.send()
>> temp1.vbs ECHO If objXMLHTTP.Status = 200 Then
>> temp1.vbs ECHO WScript.Echo "Got response. Processing body..."
>> temp1.vbs ECHO Set objADOStream = CreateObject("ADODB.Stream")
>> temp1.vbs ECHO objADOStream.Open
>> temp1.vbs ECHO objADOStream.Type = 1
>> temp1.vbs ECHO objADOStream.Write objXMLHTTP.ResponseBody
>> temp1.vbs ECHO objADOStream.Position = 0
>> temp1.vbs ECHO Set objFSO = Createobject("Scripting.FileSystemObject")
>> temp1.vbs ECHO If objFSO.Fileexists(strHDLocation) Then objFSO.DeleteFile strHDLocation
>> temp1.vbs ECHO Set objFSO = Nothing
>> temp1.vbs ECHO WScript.Echo "Saving result to a file..."
>> temp1.vbs ECHO objADOStream.SaveToFile strHDLocation
>> temp1.vbs ECHO objADOStream.Close
>> temp1.vbs ECHO Set objADOStream = Nothing
>> temp1.vbs ECHO WScript.Echo "Success."
>> temp1.vbs ECHO End if
>> temp1.vbs ECHO Set objXMLHTTP = Nothing

if exist %windir%\Microsoft.NET\Framework\v2.0.50727\NUL (
    %windir%\Microsoft.NET\Framework\v2.0.50727\csc temp.cs
) else (
if exist %windir%\Microsoft.NET\Framework\v4.0.30319\NUL (
    %windir%\Microsoft.NET\Framework\v4.0.30319\csc temp.cs
) else (
    goto USE_VBS_AS_FALLBACK
))
temp.exe
del temp.exe
goto :EXTRACT

:USE_VBS_AS_FALLBACK
cscript temp1.vbs %prefix%/%filename%.zip %filename%.zip

:EXTRACT
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
del temp.cs
del temp1.vbs
del temp2.vbs
del %filename%.zip
echo Done.
