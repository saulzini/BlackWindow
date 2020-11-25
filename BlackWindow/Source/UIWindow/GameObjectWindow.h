#pragma once
#include "DefaultImGuiWindow.h"
class GameObjectWindow : public DefaultImGuiWindow
{
public:
    GameObjectWindow(
        const char *_title,
        ImGuiWindowFlags _window_flags) : DefaultImGuiWindow(_title, _window_flags) {};
    void Update() override;

    void DrawTransformation();

    void DrawGeometry();

    void DrawTexture();
};
