workspace "gameplay"

    local project_action = "UNDEFINED"
    if _ACTION ~= nill then project_action = _ACTION end
    
    local platform = "%{cfg.system}-%{cfg.platform}"

    local target_dir = path.join(os.getcwd(), "_build/"..platform.."/%{cfg.buildcfg}")
    local workspace_dir = path.join(os.getcwd(), "_compiler/"..project_action)
    local deps_dir = path.join(os.getcwd(), "_deps")
    local vulkan_sdk_dir = os.getenv("VULKAN_SDK")
   
    configurations { "debug", "release" }
    startproject "gameplay.editor"
    location (workspace_dir)
    targetdir (target_dir)
    objdir("_build/tmp/%{cfg.system}/%{prj.name}")
    symbols "On"
    exceptionhandling "On"
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
        buildoptions { "-fvisibility=hidden -D_FILE_OFFSET_BITS=64 -fPIC" }
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
            vulkan_sdk_dir.."/include",
            deps_dir.."/imgui", 
            deps_dir.."/freetype/include",
            deps_dir.."/spdlog/include",
            deps_dir.."/cpptoml/include",
            deps_dir.."/stb/include",
            deps_dir.."/glfw/include",
            deps_dir.."/glm/include",
        }
        libdirs {
            vulkan_sdk_dir.."/lib",
        }
        links { "freetype", "glfw3" }
        filter { "system:windows" }
            libdirs {
                deps_dir.."/freetype/bin/"..platform.."/%{cfg.buildcfg}",
                deps_dir.."/glfw/bin/"..platform.."/%{cfg.buildcfg}",
            }
            links { "vulkan-1", "opengl32", "user32", "gdi32", "shell32", "kernel32", "pathcch" }
        filter { "system:linux" }
            libdirs {
                deps_dir.."/freetype/bin/"..platform,
                deps_dir.."/glfw/bin/"..platform,
            }
            links { "vulkan", "dl", "pthread", "X11", "z"}
            linkoptions { "-export-dynamic" }
            buildoptions { "-pthread" }
            disablewarnings { "unused-function", "unused-value", "unused-but-set-variable" }
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
        dependson( "gameplay" )
        kind "ConsoleApp"
        location (workspace_dir.."/%{prj.name}")
        includedirs("include")
        libdirs { target_dir }
        links { "gameplay" }
        filter { "system:linux" }
            links { "dl", "pthread" }
            buildoptions { "-pthread" }
        filter {}
        files { "source/gameplay.editor/*.cpp" }
