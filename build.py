##############################################################################
# Packaging script for building and packaging gameplay-deps-<platform>.zip 
##############################################################################
import os
import glob
import platform
import subprocess
import shutil
import sys
import time
import zipfile
from pathlib import Path
from distutils.dir_util import copy_tree

# constants
##############################################################################

TOOLS_FOLDER = "_tools"
COMPILER_FOLDER = "_compiler"
BUILD_FOLDER = "_build"

# platform-architecture
##############################################################################
platform_arch = ""
if sys.platform == "win32":
    platform_arch = "windows-x86_64"
elif sys.platform == "darwin":
    platform_arch = "macos-x86_64"
else:
    platform_arch = "linux-x86_64"


# function utils
##############################################################################

def clear_dir(dir_path):
    if os.path.exists(dir_path):
        shutil.rmtree(dir_path, ignore_errors=True)
    path = Path(dir_path)
    path.mkdir(parents=True)

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

# arguments
argc = len(sys.argv)
generate_only = False
arg0 = ""
if argc > 1:
    arg0 = sys.argv[1]
    if arg0 == "-g":
        generate_only = True

# generate/premake
current_dir = os.getcwd()
compiler_args = ""
if sys.platform == "win32":
    compiler_args = "vs2019"
tools_dir = os.path.join(current_dir, TOOLS_FOLDER)
premake_proc = subprocess.Popen(f"{tools_dir}/premake/premake5 --file=premake5.lua {compiler_args}", cwd=current_dir)
premake_proc.wait()

# compile/link
if not generate_only:
    compiler_dir =  os.path.join(current_dir, COMPILER_FOLDER)
    if sys.platform == "win32":
        compiler_dir = os.path.join(compiler_dir, "vs2019")
        init_vsvars()
        os.chdir(compiler_dir)
        build_cmd = f"msbuild gameplay.sln /property:Configuration=Debug"
        subprocess.run(build_cmd)
        build_cmd = f"msbuild gameplay.sln /property:Configuration=Release"
        subprocess.run(build_cmd)
    else:
        pass
