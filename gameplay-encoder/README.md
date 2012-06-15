## gameplay-encoder
Command-line tool for encoding games assets like true-type fonts and 3D scene files
into a simple binary-based bundle file format for the gameplay 3D game framework runtime. 
The 'bin' folder contains a pre-built version of the gameplay-encoder with support built-in support for:

## TrueType Font Support
TrueType Fonts conversion is enabled/built-in by default into gameplay-encoder via freetype 2 library.

## COLLADA Scene Support
COLLADA is enabled/built-in by default into gameplay-encoder via COLLADA-DOM library.
Most major 3D DCC tools support the export of COLLADA 1.4.
We also recommend you download and use OpenCOLADA (http://opencollada.org/)
for Autodesk Maya and 3DS Max.

## FBX Scene Support
FBX support can easily be enabled in gameplay-encoder but requires an 
additional installation of Autodesk FBX SDK. (http://www.autodesk.com/fbx).

You must then rebuild gameplay-encoder with the follow platform/tooling instructions:

### Building FBX Support on Windows 7 using Visual Studio 2010
- Download and install the FBX SDK for Window VS2010. (http://www.autodesk.com/fbx)
- Edit the project properties of "gameplay-encoder"
- Add Preprocessor Definition "USE_FBX" (C++/Preprocessor)
- Add the FBX SDK include directory to Additional Include Directories (C++/General)
  * Example: C:/Program Files/Autodesk/FBX/FbxSdk/2013.1/include
- Add the FBX lib directory to the Additional Library Directories (Linker/General)
  * Example: C:/Program Files/Autodesk/FBX/FbxSdk/2013.1/lib/vs2010/x86
- Add "fbxsdk-2013.1-md.lib"(Release) and "wininet.lib" to the Additional Dependencies (Linker/Input)
  * Example: fbxsdk-2013.1-md.lib;wininet.lib
- Build gameplay-encoder

### Building FBX Support on Mac OS X using XCode 4.3.2+
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
