#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "UIWindow/ConsoleWindow.h"
#include "UIWindow/ConfigurationWindow.h"
#include "UIWindow/PreviewWindow.h"
#include "UIWindow/HierarchyWindow.h"
#include "UIWindow/GameObjectWindow.h"
#include "UIWindow/AboutWindow.h"
#include "Leaks.h"
ModuleEditor::ModuleEditor()
{
    showDemoWindow = false;
    showDock = true;
    appStatus = UPDATE_CONTINUE;
}

ModuleEditor::~ModuleEditor()
{
    
}

bool ModuleEditor::Init()
{
    //Creating windows
    AddWindow(consoleWindow = new ConsoleWindow("Console window", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground));
    AddWindow(configurationWindow = new ConfigurationWindow("Configuration window", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground));
    AddWindow(previewWindow = new PreviewWindow("Preview", ImGuiWindowFlags_MenuBar ));
    AddWindow(hierarchyWindow = new HierarchyWindow("Hierarchy", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground));
    AddWindow(gameObjectWindow = new GameObjectWindow("GameObject", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground));
    AddWindow(aboutWindow = new AboutWindow("About", ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    SetStyle(io);
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext());
    ImGui_ImplOpenGL3_Init();

    return true;
}

update_status ModuleEditor::PreUpdate(float deltaTime)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    ShowDockSpace(&showDock);

    return appStatus;
}

update_status ModuleEditor::Update(float deltaTime)
{
    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    UpdateWindows();

    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float deltaTime)
{
    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    LOG("Editor module clean");

    //Cleaning vector
    for (unsigned int i = 0; i < windows.size(); ++i)
    {
        windows[i]->CleanUp();
        delete (windows[i]);
        windows[i] = nullptr;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::AddWindow(DefaultImGuiWindow *window)
{
    windows.push_back(window);
}

void ModuleEditor::SetStyle(const ImGuiIO& io)
{
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
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

void ModuleEditor::ShowDockSpace(bool *pOpen)
{
    static bool optFullScreenPersistant = true;
    bool optFullscreen = optFullScreenPersistant;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (optFullscreen)
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
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
    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("###DockSpace", pOpen, window_flags);
    ImGui::PopStyleVar();

    if (optFullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    DrawMenu();
    ImGui::End();
}

void ModuleEditor::DrawMenu()
{

    //Adding help menu bar
    if (ImGui::BeginMenuBar())
    {
        // Menu for view windows
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Console window"))
                consoleWindow->toggleWindow();
            if (ImGui::MenuItem("Hierarchy window"))
                hierarchyWindow->toggleWindow();
            if (ImGui::MenuItem("Config window"))
                configurationWindow->toggleWindow();
            if (ImGui::MenuItem("GameObject window"))
                gameObjectWindow->toggleWindow();
            if (ImGui::MenuItem("About window"))
                aboutWindow->toggleWindow();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Gui Demo"))
            {
                //open demo
                showDemoWindow = !showDemoWindow;
            }
            //  showcase != showcase;
            if (ImGui::MenuItem("Documentation"))
                App->RequestBrowser("https://github.com/saulzini/BlackWindow/wiki");
            if (ImGui::MenuItem("Download latest"))
                App->RequestBrowser("https://github.com/saulzini/BlackWindow/releases");
            if (ImGui::MenuItem("Report a bug"))
                App->RequestBrowser("https://github.com/saulzini/BlackWindow/issues");
            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Quit"))
            appStatus = UPDATE_STOP;
        ImGui::EndMenuBar();
    }
}
