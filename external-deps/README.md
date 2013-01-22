## GamePlay External Dependencies

A pre-built version of all required external dependencies is included here for convenience. However, you may at some point wish to rebuild some or all of the external depencies if for example you want to target a different processor, build with a different character encoding, etc.

Each external library folder includes a 'README.md' file that lists the current version of the external library that is expected by GamePlay, so ensure you grab the correct source code version when building libraries.

Generally you should build external libraries in Release mode, not Debug, unless you have a specific need to debug the third party libraries.

Platform-specific instructions are below.

## Windows (Microsoft Visual Studio)

1. Select the character encoding to match GamePlay libraries (Unicode by default)

2. Turn off Whole Program Optimization on General page

3. Make sure Multi-threaded DLL (/MD) is specified on C/C++ -> Code Generation page

4. Make sure the "Debug Information Format" option is cleared (no /Zx option specified on command-line) on the C/C++ -> General page
