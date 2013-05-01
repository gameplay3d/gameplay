## Usage
Make sure that you have [doxygen](http://www.doxygen.org/) installed.

To generate the Lua script bindings for gameplay, run the generate-doxygen-xml.bat (or .sh) script. Then, on Windows, open the gameplay-luagen Visual Studio solution and build and run the gameplay-luagen project. On Mac, open the gameplay-luagen XCode workspace and build and run the gameplay-luagen project.

There are also prebuilt binaries in the gameplay/bin folder.


## Unsupported Features
- operators
- templates
- variable arguments
- Lua doesn't support as many types as C++ so if there are functions that overload on parameters with types that overlap in Lua, the overloading won't work properly (i.e. char, short, int, long, float, double and all corresponding unsigned variants overlap in Lua).


### To Do List
- Look into updating bindValue() to support binding to any Lua script function.
- Add a global function that implements casting for use from Lua scripts (i.e. to downcast from a Control to a Button).
- Currently ignored: there is one memory leak in gameplay-luagen that is very difficult to fix (it appears to be a locale related leak-something leaks the first time an ofstream is created-doesn't matter the ofstream).


## Disclaimer
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
