## gameplay-encoder
Command-line tool for encoding games assets like true-type fonts and 3D scene files
into a simple binary-based bundle file format for the gameplay 3D game framework runtime. 
The 'bin' folder contains pre-built versions of the gameplay-encoder executables for both 
Windows 7 and MacOS X with support built-in support for:

## TrueType Font
TrueType Fonts represent a standard in defining outline fonts and has become the 
most common format for fonts. The gameplay-encoder reads these fonts and binary encodes 
them into a texture mapped base representation using a texture atlas and 8-bit alpha
representation.

## COLLADA Scene
COLLADA is a royalty-free XML schema that enables digital asset exchange 
within the interactive 3D industry. Most major 3D DCC tools support export to COLLADA 1.4.

## FBX Scene
Autodesk® FBX® asset exchange technology facilitates higher-fidelity data exchange 
between several Autodesk content creation packages
Autodesk® Maya®, Autodesk® 3ds Max®, Autodesk® MotionBuilder®, Autodesk® Mudbox®, and Autodesk® Softimage®
For more information goto "http://www.autodesk.com/fbx".

## Building gameplay-encoder
The gameplay-encoder comes pre-built for both Windows 7 and MacOS X in the 'bin' folder.
However, to build the gameplay-ecoder yourself just open either the 
Visual Studio 2010 project "gameplay-encoder.vccproj" on Windows 7 or
XCode project "gameplay-encoder.xcodeproj" on MacOSX.

### Building with FBX Support on Windows 7 using Visual Studio 2010
- Download and install the FBX SDK for Window VS2010. (http://www.autodesk.com/fbx)
- Edit the project properties of "gameplay-encoder" for Debug
- Add Preprocessor Definition "USE_FBX" (C++/Preprocessor)
- Add the FBX SDK include directory to Additional Include Directories (C++/General)
  * Example: C:/Program Files/Autodesk/FBX/FbxSdk/2013.1/include
- Add the FBX lib directory to the Additional Library Directories (Linker/General)
  * Example: C:/Program Files/Autodesk/FBX/FbxSdk/2013.1/lib/vs2010/x86
- Add "fbxsdk-2013.1-mdd.lib"(Release) and "wininet.lib" to the Additional Dependencies (Linker/Input)
  * Example: fbxsdk-2013.1-mdd.lib;wininet.lib
- Build gameplay-encoder

### Building with FBX Support on Mac OS X using XCode 4.3.2+
- Download and install the FBX SDK for Mac OS X (http://www.autodesk.com/fbx)
- Edit the project properties of target "gameplay-encoder".
- Add Preprocessor Macro "USE_FBX" to both Debug/Release sections. (Build Settings)
- Add the FBX include directory to Header Search Paths: (Build Settings)
  * Example: /Applications/Autodesk/FBXSDK20131/include
- Add the FBX library and dependency Library/Frameworks: (Build Phases -> Link Binary with Libraries)
  * Example: /Applications/Autodesk/FBXSDK20131/lib/gcc4/ub/libfbxsdk-2013.1-static.a  (Add Other)
  * Example: libiconv.dylib, Cocoa.framework, SystemConfiguration.framework
- Build gameplay-encoder

## Bundle File Format
The gameplay bundle file format is well defined in the gameplay-encoder/gameplay-bundle.txt file.

## Bundle File Loading
Bundle files can easily be loaded using the gameplay/Bundle.h which is part of the gameplay runtime framework.

## Disclaimer
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
