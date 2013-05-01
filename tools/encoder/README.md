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

See [Building gameplay-encoder](https://github.com/blackberry/GamePlay/wiki/Building-gameplay-encoder) on the wiki.

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
