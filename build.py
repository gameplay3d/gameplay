##############################################################################
# build.py script
##############################################################################
import os
import argparse
import glob
import subprocess
import shutil
import sys
from pathlib import Path

# build.py arguments
##############################################################################
parser = argparse.ArgumentParser()
parser.add_argument("-c", "--clean", help="Clean only", action="store_true")
parser.add_argument("-g", "--generate", help="Generates only", action="store_true")
parser.add_argument("-b", "--build", help="Builds only", action="store_true")
parser.add_argument("--configuration", help="Specify the configuration. (all,debug,release)", default="all")
parser.add_argument("--toolchain", help="Toolchain override.", default="")
args = parser.parse_args()

# constants
##############################################################################

BUILD_FOLDER = "_build"
COMPILER_FOLDER = "_compiler"
DEPS_FOLDER = "_deps"

# utility functions
##############################################################################

def clear_dir(dir_path):
    if os.path.exists(dir_path):
        shutil.rmtree(dir_path, ignore_errors=True)
    path = Path(dir_path)
    path.mkdir(parents=True)
    if os.path.exists(dir_path):
        os.rmdir(dir_path)

def copy_files(src_dir, dst_dir, match_exp):
    clear_dir(dst_dir)
    for filename in glob.glob(os.path.join(src_dir, match_exp)):
        shutil.copy(filename, dst_dir)

def remove_files(src_dir, match_exp):
    for filename in glob.glob(os.path.join(src_dir, match_exp)):
        os.remove(filename)

def init_vsvars():
    vswhere_path = r"%ProgramFiles(x86)%/Microsoft Visual Studio/Installer/vswhere.exe"
    vswhere_path = os.path.expandvars(vswhere_path)
    if not os.path.exists(vswhere_path):
        raise EnvironmentError("vswhere.exe not found at: %s", vswhere_path)
    vs_path = os.popen('"{}" -latest -property installationPath'.format(vswhere_path)).read().rstrip()
    vsvars_path = os.path.join(vs_path, "VC\\Auxiliary\\Build\\vcvars64.bat")
    output = os.popen('"{}" && set'.format(vsvars_path)).read()
    for line in output.splitlines():
        pair = line.split("=", 1)
        if(len(pair) >= 2):
            os.environ[pair[0]] = pair[1]

# building
##############################################################################

current_dir = os.getcwd()

# generate premake for toolchain (specified or host detected)
if args.toolchain:
    toolchain = args.toolchain
else:
    if sys.platform == "win32":
        toolchain = "vs2019"
    elif sys.platform == "linux":
        toolchain = "gmake"
    elif sys.platform == "darwin":
        toolchain = "xcode4"

# premake
if not args.build and not args.clean:
    deps_dir = os.path.join(current_dir, DEPS_FOLDER)
    premake_proc = subprocess.Popen(f"{deps_dir}/premake/premake5 --file=premake5.lua {toolchain}", cwd=current_dir, shell=True)
    premake_proc.wait()

# clean build check
clean_build = False
if args.clean:
    clean_build = True

# build configurations
config_debug = False
config_release = False
if args.configuration == "all":
    config_debug = True
    config_release = True
elif args.configuration == "debug":
    config_debug = True
elif args.configuration == "release":
    config_release = True

# build compiler/toolchain (skip if generate_only is specified)
build_dir = os.path.join(current_dir, BUILD_FOLDER)
if not args.generate:
    compiler_dir = os.path.join(current_dir, COMPILER_FOLDER)
    if toolchain == "vs2019":
        compiler_dir = os.path.join(compiler_dir, "vs2019")
        init_vsvars()
        os.chdir(compiler_dir)
        if clean_build:
            subprocess.run("msbuild gameplay.sln -t:Clean")
            clear_dir(build_dir)
        else:
            if config_debug:
                subprocess.run("msbuild gameplay.sln /property:Configuration=Debug")
            if config_release:
                subprocess.run("msbuild gameplay.sln /property:Configuration=Release")
    elif toolchain == "gmake":
        compiler_dir = os.path.join(compiler_dir, "gmake")
        os.chdir(compiler_dir)
        if clean_build:
            subprocess.run("make clean", shell=True)
            clear_dir(build_dir)
        else:
            if config_debug:
                subprocess.run("make config=debug_x86_64", shell=True)
            if config_release:
                subprocess.run("make config=release_x86_64", shell=True)
    elif toolchain == "xcode4":
        compiler_dir = os.path.join(compiler_dir, "xcode4")
        os.chdir(compiler_dir)
        if clean_build:
            subprocess.run("xcodebuild clean -workspace gameplay.xcworkspace", shell=True)
            clear_dir(build_dir)
        else:
            if config_debug:
                subprocess.run("xcodebuild -workspace gameplay.xcworkspace -configuration Debug build", shell=True)
            if config_release:
                subprocess.run("xcodebuild -workspace gameplay.xcworkspace -configuration Release build", shell=True)
    else:
        print("Error: Compiler toolchain not supported.")
