#include "ConfigurationWindow.h"
#include <stdio.h>  // vsnprintf, sscanf, printf
#include <stdlib.h> // NULL, malloc, free, atoi
#include <math.h>
#include "Math/float3.h"
#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
// GPU
#include "SDL_version.h"
#include "SDL_cpuinfo.h"
#include "DeviceId.h"

#include <windows.h> //memory consumption
#include "psapi.h"
void ConfigurationWindow::Update()
{

    if (show)
    {
        ImGui::SetNextWindowContentSize(ImVec2(400, 70.0f));

        if (!begin())
        {
            show = false;
            end();
            return;
        }

        DrawApplicationConfig();
        DrawWindowConfig();
        if (ImGui::CollapsingHeader("File Sytem"))
        {
        }
        DrawHardwareConfig();
        DrawCameraConfig();
        end();
    }
}

void ConfigurationWindow::DrawCameraConfig()
{
    if (ImGui::CollapsingHeader("Camera"))
    {

        if (ImGui::Button("Reset camera settings"))
        {
            App->camera->ResetCamera();
        }

        // Front
        float3 frontVector = App->camera->GetFrontVector();
        ImGui::InputFloat3("Front", &frontVector[0]);
        App->camera->SetFrontVector(frontVector);

        // Up
        float3 upVector = App->camera->GetUpVector();
        ImGui::InputFloat3("Up", &upVector[0]);
        App->camera->SetUpVector(upVector);

        // Position
        float3 cameraPosition = App->camera->GetCameraPosition();
        ImGui::InputFloat3("Position", &cameraPosition[0]);
        App->camera->SetCameraPosition(cameraPosition);

        // Move Speed
        float movementSpeed = App->camera->GetMovementSpeed();
        ImGui::DragFloat("Move Speed", &movementSpeed, 0.05);
        App->camera->SetMovementSpeed(movementSpeed);

        // Rot Speed
        float turnSpeed = App->camera->GetTurnSpeed();
        ImGui::DragFloat("Rot Speed", &turnSpeed, 0.05);
        App->camera->SetTurnSpeed(turnSpeed);

        //Zoom Speed
        float zoomSpeed = App->camera->GetZoomSpeed();
        ImGui::DragFloat("Zoom Speed", &zoomSpeed, 1);
        App->camera->SetZoomSpeed(zoomSpeed);

        // Frustrum Culling
        // static bool fCulling = false;
        // ImGui::Checkbox("Frustrum Culling", &fCulling);

        // Near Plane
        float nearPlane = App->camera->GetNearPlaneDistance();
        ImGui::DragFloat("Near Plane", &nearPlane, 0.5);
        App->camera->SetNearPlaneDistance(nearPlane);

        // Far Plane
        float farPlane = App->camera->GetFarPlaneDistance();
        ImGui::DragFloat("Far Plane", &farPlane, 0.5);
        App->camera->SetFarPlaneDistance(farPlane);

        // Horizontal Field of View
        float horizontalFOV = App->camera->GetHorizontalDegreesFieldOfView();
        ImGui::DragFloat("Horizontal (DEG) FOV", &horizontalFOV, 0.05);
        App->camera->SetHorizontalDegreesFieldOfView(horizontalFOV);

        // Aspect Ratio
        float aspectRatio = App->camera->GetAspectRatio();
        ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.05);
        App->camera->SetAspectRatio(aspectRatio);
    }
}

void ConfigurationWindow::DrawHardwareConfig()
{
    if (ImGui::CollapsingHeader("Hardware"))
    {
        // App Name
        SDL_version version;
        SDL_GetVersion(&version);
        ImGui::Text("SDL Version: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), " %d.%d.%d", version.major, version.minor, version.patch);

        ImGui::Separator();

        ImGui::Text("CPUs: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

        ImGui::Text("System RAM: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%f Gb", ((float)SDL_GetSystemRAM() / 1024.0f));

        ///Caps
        ImGui::Text("Caps: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasAVX() ? "AVX" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasAVX2() ? "| AVX2" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasAltiVec() ? "| AltiVec" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasMMX() ? "| MMX" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasRDTSC() ? "| RDTSC" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasSSE() ? "| SSE" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasSSE2() ? "| SSE2" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasSSE3() ? "| SSE3" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasSSE41() ? "| SSE41" : "");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", SDL_HasSSE42() ? "| SSE42" : "");
        ImGui::SameLine();
        ImGui::Separator();

        unsigned int vendor, deviceId;
        std::wstring brand;
        unsigned __int64 videoMemBudget;
        unsigned __int64 videoMemUsage;
        unsigned __int64 videoMemAvailable;
        unsigned __int64 videoMemReserved;

        if (getGraphicsDeviceInfo(&vendor, &deviceId, &brand, &videoMemBudget, &videoMemUsage, &videoMemAvailable, &videoMemReserved))
        {
            ImGui::Text("GPU: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "vendor %u device %u", vendor, deviceId);

            ImGui::Text("Brand: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%S", brand.c_str());

            ImGui::Text("VRAM Budget: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f Mb", float(videoMemBudget) / 1073741824.0f);

            ImGui::Text("VRAM Usage: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f Mb", float(videoMemUsage) / (1024.f * 1024.f * 1024.f));

            ImGui::Text("VRAM Available: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f Mb", float(videoMemAvailable) / (1024.f * 1024.f * 1024.f));

            ImGui::Text("VRAM Reserved: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f Mb", float(videoMemReserved) / (1024.f * 1024.f * 1024.f));
        }
    }
}

void ConfigurationWindow::DrawWindowConfig()
{
    if (ImGui::CollapsingHeader("Window"))
    {
        // Brightness
        float brightness = App->window->GetBrightnesss();
        ImGui::SliderFloat("Brightness", &brightness, 0, 1);
        App->window->SetBrightness(brightness);

        // Width
        int width = App->window->GetWidth();
        ImGui::DragInt("Width", &width, 1, 200, 2000);
        App->window->SetWidth(width);

        // Height
        int height = App->window->GetHeight();
        ImGui::DragInt("Height", &height, 1, 200, 2000);
        App->window->SetHeight(height);

        // Refresh rate
        ImGui::Text("Refresh rate:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%d", App->window->GetRefreshRate());

        // Options
        bool fullscreen = App->window->GetFullscreen();
        ImGui::Checkbox("Fullscreen", &fullscreen);
        App->window->SetFullscreen(fullscreen);

        ImGui::SameLine(200);
        bool resizable = App->window->GetResizable();
        ImGui::Checkbox("Resizable", &resizable);
        App->window->SetResizable(resizable);

        bool border = App->window->GetBorder();
        ImGui::Checkbox("With border", &border);
        App->window->SetBorder(border);

        ImGui::SameLine(200);
        bool fullDesktop = App->window->GetFullDesktop();
        ImGui::Checkbox("Full Desktop", &fullDesktop);
        App->window->SetFullDesktop(fullDesktop);
    }
}

void ConfigurationWindow::DrawApplicationConfig()
{
    if (ImGui::CollapsingHeader("Application"))
    { //collapsables

        // App Name
        // static char str0[128] = "Black Screen Engine";
        // ImGui::InputText("App Name", str0, IM_ARRAYSIZE(str0)); //always pass chars and size
        ImGui::Text(TITLE);
        ImGui::Spacing();
        ImGui::Text("UPC");
      
        // Max FPS
        float fps = App->GetMaxFps();
        ImGui::DragFloat("Max fps", &fps, 1.0f,1.0f,255.0f);
        App->SetMaxFps(fps);
        // Limit Frame Rate
        float* fpsResults = App->GetFpsResults();
        ImGui::Text("Limit Framerate:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%f", fpsResults[0]); //color text

        // Graph 1
        char title1[25] = "";
        sprintf_s(title1, 25, "Framerate %.1f", fpsResults[0]);
        ImGui::PlotHistogram("##framerate", fpsResults, SAMPLESFPS, 0,title1, 0.0f, 300.0f, ImVec2(310, 120)); // name (not forget #) , arr of values, size of arr, offset, min , size of col

        // Graph 2
        float* frameTimesResults = App->GetFrameTimes();
        char title2[25];
        sprintf_s(title2, 25, "Milliseconds %.1f", frameTimesResults[0]);
        ImGui::PlotHistogram("##milliseconds", frameTimesResults, SAMPLESFPS, 0, title2, 0.0f, 300.0f, ImVec2(310, 100));

        //reference : https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
        // Total Report
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        ;

        if (GlobalMemoryStatusEx(&memInfo)) {
            DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
            DWORDLONG virtualMemUsed =memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
            PROCESS_MEMORY_COUNTERS_EX pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
            SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
            DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
            DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
            SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
            ImGui::TextWrapped("Total VM available: %f MB", (float)totalVirtualMem / 1024.0f);
            ImGui::TextWrapped("VM used: %f MB", (float)virtualMemUsed / 1024.0f);
            ImGui::TextWrapped("VM used by my process: %f MB", (float)virtualMemUsedByMe / 1024.0f);
            ImGui::TextWrapped("Total RAM available: %f MB", (float)totalPhysMem / 1024.0f);

            ImGui::TextWrapped("RAM used: %f MB", (float)physMemUsed / 1024.0f);
            ImGui::TextWrapped("RAM used by my process: %f MB", (float)physMemUsedByMe / 1024.0f);
        }
        
    }
}
