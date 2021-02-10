#include "UI.h"
#include "Config.h"
#include "FileSystem.h"
#include "Window.h"
#include "WindowingGLFW.h"
#include "imgui_impl_glfw.h"


namespace gameplay
{

struct UI::Impl
{

};

UI::UI()
{
    _impl = new UI::Impl();
}

UI::~UI()
{
    GP_SAFE_DELETE(_impl);
}


void UI::startup()
{
    // setup imgui for ui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    auto window = App::get_app()->get_main_window()->handle->glfwWindow;
    ImGui_ImplGlfw_InitForVulkan(window, true);

    // load the system fonts
    auto config = App::get_app()->get_config();
    std::string defaultFont = config->get_string("ui.defaultFont", "");
    if (defaultFont.size() > 0 && App::get_app()->resolve_resource_path(defaultFont))
    {
        auto fs = App::get_app()->get_file_system();
        if (fs->exists(defaultFont.c_str()))
        {
            io.Fonts->AddFontFromFileTTF(defaultFont.c_str(), 16.0f);
        }
    }
}

void UI::shutdown()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::update()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // sample content directly with imgui for now...
    static bool show_demo_window = true;
    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::Render();
}
}
