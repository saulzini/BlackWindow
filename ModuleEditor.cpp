#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ConsoleWindow.h"
ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
    for (unsigned int i = 0; i < windows.size(); i++)
    {
        delete(&windows[i]);
    }
}

bool ModuleEditor::Init()
{
    //Creating windows
    addWindow(consoleWindow = new ConsoleWindow("Console window1", ImGuiWindowFlags_MenuBar)); //TODO:: Check how to create callback

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    SetStyle(io);
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext());
    ImGui_ImplOpenGL3_Init();

    return true;
}


void ModuleEditor::PressGuiBtn()
{
    LOG("GUI BTN");
}

void ModuleEditor::PressAboutBtn()
{
    LOG("About BTN");

}

update_status ModuleEditor::PreUpdate()
{
    bool show = true;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();


    ShowDockSpace(&show);
   // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    UpdateWindows();

    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    LOG("Editor module clean");
    ImGui_ImplOpenGL3_Shutdown(); 
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    //TODO::Clean vector
	return true;
}

void ModuleEditor::addWindow(DefaultImGuiWindow* window)
{
    windows.push_back(window);
}


void ModuleEditor::SetStyle(const ImGuiIO io)
{
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

void ModuleEditor::UpdateWindows()
{
    for (unsigned int i = 0; i < windows.size(); i++)
    {
        windows[i]->Update();
    }
}


void ModuleEditor::ShowDockSpace(bool* p_open)
{
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("###DockSpace", p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    //Adding help menu bar
    if (ImGui::BeginMenuBar()) {

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Gui Demo")) {
                //open demo
                //demo_pressed();
            }
            //  showcase != showcase;
            if (ImGui::MenuItem("Documentation"))
                App->RequestBrowser("https://github.com/saulzini/Engine/wiki");
            if (ImGui::MenuItem("Download latest"))
                App->RequestBrowser("https://github.com/saulzini/Engine/releases");
            if (ImGui::MenuItem("Report a bug"))
                App->RequestBrowser("https://github.com/saulzini/Engine/issues");
            if (ImGui::MenuItem("About")) {
                //open about
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    //End menu bar

    ImGui::End();
}
