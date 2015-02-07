## gameplay-encoder
Command-line tool for encoding games assets like true-type fonts and 3D scene files
into a simple binary-based bundle file format for the GamePlay game framework runtime. 
The 'bin' folder contains pre-built 64-bit versions of the gameplay-encoder executables for 
Windows, MacOS X and Linux Ubuntu with support built-in support for:

## TrueType Font
TrueType Fonts represent a standard in defining outline fonts and has become the 
most common format for fonts. The gameplay-encoder reads these fonts and binary encodes 
them into a texture mapped base representation using a texture atlas and 8-bit alpha
representation.

## FBX Scene
Autodesk® FBX® asset exchange technology facilitates higher-fidelity data exchange between several Autodesk content creation packages
Autodesk® Maya®, Autodesk® 3ds Max®, Autodesk® MotionBuilder®, Autodesk® Mudbox®, Autodesk® Softimage®
It is also supported on many other major 3D CAD software tools such as Blender, Sketchup, Daz, Lightwave, MODO, etc.
For more information goto: "http://www.autodesk.com/fbx".

## Running gameplay-encoder
Simply execute the gameplay-encoder command-line executable:

`Usage: gameplay-encoder [options] <file(s)>`

## Building gameplay-encoder
The tools come pre-built and are part of the install.bat/install.sh script. 
If you need to build them yourself:
See [Building gameplay-encoder](https://github.com/gameplay3d/GamePlay/wiki/Building-gameplay-encoder) on the wiki.

## Bundle File Format
The gameplay bundle file format is defined in the [tools/encoder/gameplay-bundle.txt](gameplay-bundle.txt) file.

## Bundle File Loading
Bundle files can easily be loaded using the `gameplay/Bundle.h` which is part of the gameplay runtime framework.

## Disclaimer
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
