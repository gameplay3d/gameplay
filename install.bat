@echo off
REM install.bat - Download and Extract GamePlay dependencies

set prefix=https://github.com/gameplay3d/GamePlay/releases/download/v3.0.0
set filename=gameplay-deps

REM Go to the script's directory
cd /d "%~dp0"

echo Downloading %filename%.zip from %prefix%

REM Create a C# downloader script
(
echo using System;
echo using System.Net;
echo using System.ComponentModel;
echo class Program
echo {
echo     static string file = "%filename%.zip";
echo     static string url = "%prefix%/" + file;
echo     static bool done = false;
echo     static void Main(string[] args)
echo     {
echo         try
echo         {
echo             WebClient client = new WebClient();
echo             client.Proxy = null;
echo             client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(DownloadProgressChanged);
echo             client.DownloadFileCompleted += new AsyncCompletedEventHandler(DownloadFileCompleted);
echo             Console.Write("Downloading " + file + ": 0%%    ");
echo             client.DownloadFileAsync(new Uri(url), file);
echo             while (!done) System.Threading.Thread.Sleep(500);
echo         }
echo         catch (Exception x)
echo         {
echo             Console.WriteLine("Error: " + x.Message);
echo         }
echo     }
echo     static void DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
echo     {
echo         Console.Write("\rDownloading " + file + ": " + e.ProgressPercentage + "%%    ");
echo     }
echo     static void DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
echo     {
echo         Console.WriteLine("\rDownloading " + file + ": Done.    ");
echo         done = true;
echo     }
echo }
) > temp.cs

REM Compile and run C# downloader
if exist "%windir%\Microsoft.NET\Framework\v2.0.50727\csc.exe" (
    "%windir%\Microsoft.NET\Framework\v2.0.50727\csc.exe" temp.cs
) else if exist "%windir%\Microsoft.NET\Framework\v4.0.30319\csc.exe" (
    "%windir%\Microsoft.NET\Framework\v4.0.30319\csc.exe" temp.cs
) else (
    call :UseVBScript
)

REM Clean up after C# downloader
if exist temp.exe del temp.exe
del temp.cs
goto :Extract

:UseVBScript
REM Fallback to VBScript for downloading
echo WScript.Echo "Downloading using a fallback method. This might take a few minutes." > temp1.vbs
echo Dim strFileURL, strHDLocation >> temp1.vbs
echo strFileURL = WScript.Arguments(0) >> temp1.vbs
echo strHDLocation = WScript.Arguments(1) >> temp1.vbs
echo Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP") >> temp1.vbs
echo objXMLHTTP.open "GET", strFileURL, false >> temp1.vbs
echo objXMLHTTP.send() >> temp1.vbs
echo If objXMLHTTP.Status = 200 Then >> temp1.vbs
echo     Set objADOStream = CreateObject("ADODB.Stream") >> temp1.vbs
echo     objADOStream.Open >> temp1.vbs
echo     objADOStream.Type = 1 >> temp1.vbs
echo     objADOStream.Write objXMLHTTP.ResponseBody >> temp1.vbs
echo     objADOStream.Position = 0 >> temp1.vbs
echo     Set objFSO = Createobject("Scripting.FileSystemObject") >> temp1.vbs
echo     If objFSO.Fileexists(strHDLocation) Then objFSO.DeleteFile strHDLocation >> temp1.vbs
echo     objADOStream.SaveToFile strHDLocation >> temp1.vbs
echo     objADOStream.Close >> temp1.vbs
echo     WScript.Echo "Success." >> temp1.vbs
echo End If >> temp1.vbs
cscript temp1.vbs %prefix%/%filename%.zip %filename%.zip
del temp1.vbs
goto :Extract

:Extract
echo Extracting %filename%.zip... please standby...

REM Create VBScript for extraction
echo Dim fileName, workingDir > temp2.vbs
echo fileName = WScript.Arguments(0) >> temp2.vbs
echo workingDir = CreateObject("Scripting.FileSystemObject").GetAbsolutePathName(".") >> temp2.vbs
echo Set objShell = CreateObject("Shell.Application") >> temp2.vbs
echo Set objSource = objShell.NameSpace(workingDir ^& "\" ^& fileName).Items() >> temp2.vbs
echo Set objTarget = objShell.NameSpace(workingDir ^& "\") >> temp2.vbs
echo intOptions = 256 >> temp2.vbs
echo objTarget.CopyHere objSource, intOptions >> temp2.vbs
cscript temp2.vbs %filename%.zip
del temp2.vbs

REM Clean up
echo Cleaning up...
del %filename%.zip
echo Done.
