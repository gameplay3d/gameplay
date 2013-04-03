## gameplay-encoder
Command-line tool for encoding games assets like true-type fonts and 3D scene files
into a simple binary-based bundle file format for the gameplay 3D game framework runtime. 
The 'bin' folder contains pre-built versions of the gameplay-encoder executables for 
Windows 7, MacOS X and Linux Ubuntu 12 (32-bit) with support built-in support for:

## TrueType Font
TrueType Fonts represent a standard in defining outline fonts and has become the 
most common format for fonts. The gameplay-encoder reads these fonts and binary encodes 
them into a texture mapped base representation using a texture atlas and 8-bit alpha
representation.

## FBX Scene
Autodesk® FBX® asset exchange technology facilitates higher-fidelity data exchange 
between several Autodesk content creation packages
Autodesk® Maya®, Autodesk® 3ds Max®, Autodesk® MotionBuilder®, Autodesk® Mudbox®, and Autodesk® Softimage®
For more information goto "http://www.autodesk.com/fbx".

## Running gameplay-encoder
Simply execute the gameplay-encoder command-line executable:

`Usage: gameplay-encoder [options] <file(s)>`

Note: On Linux Ubuntu 12 (64-bit), you must first install the required 32-bit libs via:

`sudo apt-get install ia32-libs`

## Building gameplay-encoder
The gameplay-encoder comes pre-built for Windows 7, MacOS X and Linux Ubuntu 12 (32-bit) in the 'bin' folder.
However, to build the gameplay-encoder yourself just open either the 
Visual Studio 2010 project "gameplay-encoder.vccproj" on Windows 7 or
XCode project "gameplay-encoder.xcodeproj" on MacOSX.
Uncomment the root CMakeList.txt for the gameplay-encoder and run standard cmake .. from build then make.

### Building with FBX Support on Windows 7 using Visual Studio 2010
- Download and install the FBX SDK for Window VS2010. (http://www.autodesk.com/fbx)
- Edit the project properties of "gameplay-encoder" for Debug
- Add Preprocessor Definition "USE_FBX" (C++/Preprocessor)
- Add the FBX SDK include directory to Additional Include Directories (C++/General)
  * Example: C:/Program Files/Autodesk/FBX/FBX SDK/2013.3/include
- Add the FBX lib directory to the Additional Library Directories (Linker/General)
  * Example: C:/Program Files/Autodesk/FBX/FBX SDK/2013.3/lib/vs2010/x86
- Add "fbxsdk-2013.3-md.lib"(Release) to the Additional Dependencies (Linker/Input)
  * Example: fbxsdk-2013.3-md.lib
- Build gameplay-encoder

### Building with FBX Support on MacOS X using XCode 4
- Download and install the FBX SDK for MacOS X (http://www.autodesk.com/fbx)
- Edit the project properties of target "gameplay-encoder".
- Add Preprocessor Macro "USE_FBX" to both Debug/Release sections. (Build Settings)
- Add the FBX include directory to Header Search Paths: (Build Settings)
  * Example: "/Applications/Autodesk/FBX SDK/2013.3/include" (Use quotes due to additional space in path)
- Add the FBX library and dependency Library/Frameworks: (Build Phases -> Link Binary with Libraries)
  * Example: /Applications/Autodesk/FBX SDK/2013.3/lib/gcc4/ub/libfbxsdk-2013.3-static.a  (Add Other)
- Build gameplay-encoder

### Building with FBX Support on Linux Ubuntu 12 (32-bit) using CMake
- Download and install the FBX SDK for MacOS X (http://www.autodesk.com/fbx)
- Edit the gameplay-encoder/CMakeLists.txt adding the following:
- Add the FBX include directory to Header Search Paths: (Build Settings)
  * Example: /usr/include/fbxsdk
- Add Preprocessor Macro to the add"-DUSE_FBX" to the end of the add_definitions(...) section of the CMakeLists.txt
- Add the FBX library path to the link_directories(...) section of the CMakeLists.txt
  * Example: /usr/lib/gcc4
- Add the FBX library to the set(APP_LIBRARIES {...} )
  * Example: fbxsdk-2013.3-static
- Add the FBX library to the library to the add_definitions(-l...) section of the CMakeLists.txt
  * Example -lfbxsdk-2013.3-static
- Build gameplay-encoder by uncommenting the last line in the gameplay/CMakeLists.txt and running the CMake build via:

```
mkdir build
cd build
cmake ..
make
 ```

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
