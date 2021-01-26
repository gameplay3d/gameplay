workspace "gameplay"

    local platform = "%{cfg.system}-%{cfg.platform}"
    local target_dir = "_build/"..platform.."/%{cfg.buildcfg}"
    local workspace_dir = "_compiler/".._ACTION
    local deps_dir = "_deps"
    local vulkan_sdk_dir = os.getenv("VULKAN_SDK")

    configurations { "debug", "release" }
    startproject "gameplay.editor"
    location (workspace_dir)
    targetdir (target_dir)
    objdir("_build/tmp/%{cfg.system}/%{prj.name}")
    symbols "On"
    exceptionhandling "Off"
    rtti "On"
    staticruntime "On"
    flags { "FatalCompileWarnings", "MultiProcessorCompile", "NoPCH", "UndefinedIdentifiers", "NoIncrementalLink" }
    cppdialect "C++17"
    systemversion "latest"
    filter { "system:windows" }
        platforms { "x86_64" }
        files { ".editorconfig" }
        editandcontinue "Off"
        symbols "Full"
        buildoptions { "/utf-8", "/bigobj" }
        buildoptions { "/permissive-" }
        buildoptions { "/WX" }
        warnings "Extra"
        disablewarnings { "4100", "4127", "4189", "4201", "4251", "4530" }
    filter { "system:linux" }
        platforms { "x86_64" }
        defaultplatform "x86_64"
        buildoptions { "-fvisibility=hidden -D_FILE_OFFSET_BITS=64" }
        linkoptions { "-Wl,-rpath, '$$ORIGIN' -Wl, --export-dynamic" }
        enablewarnings { "all", "vla" }
        disablewarnings { "error=unused-variable" }
    filter { "system:linux", "configurations:debug" }
        buildoptions { "-fstack-protector-strong" }
    filter { "platforms:x86_64" }
        architecture "x86_64"
    filter { "configurations:debug", "system:windows" }
        optimize "Off"
    filter { "configurations:debug", "system:linux" }
        optimize "Off"
    filter  { "configurations:release" }
        defines { "NDEBUG" }
    filter  { "configurations:release", "system:windows" }
        optimize "Speed"
    filter { "configurations:release", "system:linux" }
        optimize "On"
    filter {}

    project "gameplay"
        kind "SharedLib"
        location (workspace_dir.."/%{prj.name}")
        defines { "GP_EXPORT" }
        includedirs {
            "include/gameplay",
            "source/gameplay",
            vulkan_sdk_dir.."/Include",
            deps_dir.."/imgui", 
            deps_dir.."/freetype/include",
            deps_dir.."/spdlog/include",
            deps_dir.."/glfw/include",
            deps_dir.."/glm/include",
            deps_dir.."/cpptoml/include",
            deps_dir.."/stb/include",
        }
        libdirs { 
            target_dir,
            vulkan_sdk_dir.."/lib",
            deps_dir.."/freetype/bin/"..platform.."/%{cfg.buildcfg}",
            deps_dir.."/glfw/bin/"..platform.."/%{cfg.buildcfg}",
            
        }
        links { "vulkan-1", "freetype", "glfw3" }
        filter { "system:windows", "platforms:x86_64" }
            links { "opengl32", "user32", "gdi32", "shell32", "kernel32", "pathcch" }
        filter { "system:linux" }
            buildoptions { "-pthread" }
            links { "dl", "pthread" }
            disablewarnings { "error=unused-function" }
        filter {}
        files {
            "include/gameplay/*.*", 
            "source/gameplay/*.*",
             deps_dir.."/imgui/*.h", 
             deps_dir.."/imgui/imgui.cpp",
             deps_dir.."/imgui/imgui_demo.cpp",
             deps_dir.."/imgui/imgui_draw.cpp",
             deps_dir.."/imgui/imgui_tables.cpp",
             deps_dir.."/imgui/imgui_widgets.cpp",
             deps_dir.."/imgui/imgui_widgets.cpp",
             deps_dir.."/imgui/misc/freetype/imgui_freetype.*",
        }
        vpaths 
        {
           ["imgui"] = deps_dir.."/imgui/**.*",
           ["include"] = "include/gameplay/*.*",
           ["source"] = "source/gameplay/*.*",
        }

    project "gameplay.editor"
        kind "ConsoleApp"
        location (workspace_dir.."/%{prj.name}")
        includedirs("include")
        libdirs { target_dir }
        links { "gameplay" }
        filter { "system:linux" }
            buildoptions { "-pthread" }
            links { "dl", "pthread" }
            disablewarnings { "error=unused-function" }
        filter {}
        files { "source/gameplay.editor/*.cpp" }

