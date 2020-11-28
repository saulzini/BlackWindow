#pragma once
#include "DefaultImGuiWindow.h"
class Model;
class GameObjectWindow : public DefaultImGuiWindow
{
public:
    GameObjectWindow(
        const char *_title,
        ImGuiWindowFlags _window_flags) : DefaultImGuiWindow(_title, _window_flags) {
            model = nullptr;
        };

    void Update() override;

    void DrawTransformation();

    void DrawGeometry();

    void DrawTexture();
private: 
    Model* model;

};
