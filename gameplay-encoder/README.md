## GamePlay Encoder

GamePlay Encoder is a command line tool for converting COLLADA and FBX scene files into a binary file format for GamePlay.

## How to Enable FBX in Visual Studio

- Download and install the FBX SDK (http://www.autodesk.com/fbx)
- Edit the project properties of "gameplay-encoder"
- Add Preprocessor Definition "USE_FBX" (C++/Preprocessor)
- Add the FBX SDK include directory to Additional Include Directories (C++/General)
  * Example: C:/Program Files/Autodesk/FBX/FbxSdk/2012.2/include
- Add the FBX lib directory to the Additional Library Directories (Linker/General)
  * Example: C:/Program Files/Autodesk/FBX/FbxSdk/2012.2/lib/vs2010/x86
- Add "fbxsdk-2012.2-mdd.lib" and "wininet.lib" to the Additional Dependencies (Linker/Input)
  * Example: fbxsdk-2012.2-mdd.lib;wininet.lib
- Add a post build event to copy the DLL (Build Events/Post-Build Event)
  * Example: copy /Y "C:\Program Files\Autodesk\FBX\FbxSdk\2012.2\lib\vs2010\x86\fbxsdk-2012.2d.dll" "$(TargetDir)"
- Build gameplay-encoder

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
