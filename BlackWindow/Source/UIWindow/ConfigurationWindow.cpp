#include "ConfigurationWindow.h"
#include <stdio.h>  // vsnprintf, sscanf, printf
#include <stdlib.h> // NULL, malloc, free, atoi
#include "Application.h"
#include "ModuleCamera.h"
#include <math.h>
#include "Math/float3.h"
// GPU
#include "SDL_version.h"
#include "SDL_cpuinfo.h"
#include "DeviceId.h"
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

        ImGui::Text("Console window");
        ImGui::Spacing(); //new line
        if (ImGui::CollapsingHeader("Application"))
        { //collapsables

            // App Name
            static char str0[128] = "Black Screen Engine";
            ImGui::InputText("App Name", str0, IM_ARRAYSIZE(str0)); //always pass chars and size

            // Organization
            static char str1[128] = "UPC";
            ImGui::InputText("Organization", str1, IM_ARRAYSIZE(str1));

            // Max FPS
            static int i1 = 0;
            static int max1 = 255;
            ImGui::SliderInt("slider int", &i1, 1, max1); //slider value, int step, int max

            // Limit Frame Rate
            ImGui::Text("Limit Framerate:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%d", max1); //color text

            // Graph 1
            // static int values_offset = 0;
            static float arr[] = {0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f, 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f, 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f};
            char title1[25];
            sprintf_s(title1, 25, "Framerate %.1f", 20);
            ImGui::PlotHistogram("##framerate", arr, IM_ARRAYSIZE(arr), 0, title1, 0.0f, 1.0f, ImVec2(310, 100)); // name (not forget #) , arr of values, size of arr, offset, min , size of col

            // Graph 2
            char title2[25];
            sprintf_s(title2, 25, "Milliseconds %.1f", 20);
            ImGui::PlotHistogram("##milliseconds", arr, IM_ARRAYSIZE(arr), 0, title2, 0.0f, 1.0f, ImVec2(310, 100));

            // Total Report
            ImGui::Text("Total Reported Mem: %d", 1);
            ImGui::Text("Total Actual Mem: %d", 1);
            ImGui::Text("Peak Reported Mem: %d", 1);
            ImGui::Text("Peak Actual Mem: %d", 1);
            ImGui::Text("Accumulated Reported Mem: %d", 1);
            ImGui::Text("Accumulated Actual Mem: %d", 1);
            ImGui::Text("Accumulated Alloc Unit Count: %d", 1);
            ImGui::Text("Accumulated Unit Count: %d", 1);
            ImGui::Text("Peak Alloc Unit Count: %d", 1);
        }
        if (ImGui::CollapsingHeader("Window"))
        {

            // Active
            static bool wActive = false;
            ImGui::Checkbox("Active", &wActive);

            //Icon
            ImGui::Text("Icon: %s", "ddwdw");

            // Brightness
            static int brightness = 0;
            static int brightnessMax = 255;
            ImGui::SliderInt("Brightness", &brightness, 1, brightnessMax);

            // Width
            static int width = 0;
            static int widthMax = 255;
            ImGui::SliderInt("Width", &width, 1, widthMax);

            // Height
            static int height = 0;
            static int heightMax = 255;
            ImGui::SliderInt("Height", &height, 1, heightMax);

            // Refresh rate
            static int i1 = 0;
            static int max1 = 50;
            ImGui::Text("Refresh rate:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%d", max1);

            // Options
            static bool fullscreen;
            static bool resizable;
            static bool borderless;
            static bool fullDesktop;

            ImGui::Checkbox("Fullscreen", &fullscreen);
            ImGui::SameLine(300);
            ImGui::Checkbox("Resizable", &resizable);
            ImGui::Checkbox("Borderless", &borderless);
            ImGui::SameLine(300);
            ImGui::Checkbox("Full Desktop", &fullDesktop);
        }
        if (ImGui::CollapsingHeader("File Sytem"))
        {
        }
        if (ImGui::CollapsingHeader("Input"))
        {
        }
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
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%d Gb", (float)SDL_GetSystemRAM() / (1024.f));

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
