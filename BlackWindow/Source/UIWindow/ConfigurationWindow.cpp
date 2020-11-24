#include "ConfigurationWindow.h"
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#include "Application.h"
#include "ModuleCamera.h"
#include <math.h>

// GPU
#include "SDL/include/SDL_version.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "resources/gpudetect/DeviceId.h"

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
        if (ImGui::CollapsingHeader("Application")) { //collapsables

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
            static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f, 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f, 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
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
        if (ImGui::CollapsingHeader("Window")) {

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

            ImGui::Checkbox("Fullscreen", &fullscreen); ImGui::SameLine(300);
            ImGui::Checkbox("Resizable", &resizable);
            ImGui::Checkbox("Borderless", &borderless); ImGui::SameLine(300);
            ImGui::Checkbox("Full Desktop", &fullDesktop);

        }
        if (ImGui::CollapsingHeader("File Sytem")) {}
        if (ImGui::CollapsingHeader("Input")) {}
        if (ImGui::CollapsingHeader("Hardware")) {

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
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%d Gb", (float) SDL_GetSystemRAM() / (1024.f));

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
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%S",  brand.c_str());

                ImGui::Text("VRAM Budget: ");
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%.1f Mb", float(videoMemBudget) /  1073741824.0f);

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
        if (ImGui::CollapsingHeader("Camera")) {

            // Active
            static bool wActive = false;
            ImGui::Checkbox("Active", &wActive);

            // Front
            static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };

            ImGui::InputFloat3("Front", vec4f);

            // Up
            ImGui::InputFloat3("Up", vec4f);

            // Position
           // ImGui::InputFloat3("Position", vec4f);

            
            //ImGui::InputFloat3("Position", App->camera->cameraPosition[0] );
            ImGui::InputFloat3("Position", &App->camera->cameraPosition.x);

            // Move Speed
            static float ms = 0.f;
            ImGui::DragFloat("Move Speed", &ms, 1);

            // Rot Speed
            static float rs = 0.f;
            ImGui::DragFloat("Rot Speed", &rs, 1);

            //Zoom Speed
            static float zs = 0.f;
            ImGui::DragFloat("Zoom Speed", &zs, 1);

            // Frustrum Culling
            static bool fCulling = false;
            ImGui::Checkbox("Frustrum Culling", &fCulling);

            // Near Plane
            static float np = 0.f;
            ImGui::DragFloat("Near Plane", &np, 1);

            // Far Plane
            static float fp = 0.f;
            ImGui::DragFloat("Far Plane", &fp, 1);

            // Field of View
            static float fv = 0.f;
            ImGui::DragFloat("Field of View", &fv, 1);

            // Aspect Ratio
            static float ar = 0.f;
            ImGui::DragFloat("Aspect Ratio", &ar, 1);

            // Background
            static float col1[3] = { 1.0f, 0.0f, 0.2f };
            ImGui::ColorEdit3("color 1", col1);

            // Current
            ImGui::Text("Current ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", "- not assigned-");

            // Pick Another
            ImGui::PushID(1); //begin btn
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
            ImGui::Button("Pick Another");
            ImGui::PopStyleColor(3);
            ImGui::PopID(); //end btn

            // Is Active Camera
            static bool isActive = false;
            ImGui::Checkbox("Is Active Camera", &isActive);

        }
        end();
    }
}
