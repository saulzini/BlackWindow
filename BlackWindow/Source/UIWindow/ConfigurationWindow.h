#pragma once
#include "DefaultImGuiWindow.h"
#include "Application.h"
#include "ModuleWindow.h"

class ConfigurationWindow :
	public DefaultImGuiWindow
{
public:
	ConfigurationWindow(
		const char* _title,
		ImGuiWindowFlags _window_flags
	) : DefaultImGuiWindow(_title, _window_flags) {
	};
	void Update() override;

protected:
	void DrawCameraConfig();
	void DrawHardwareConfig();
	void DrawWindowConfig();
	void DrawApplicationConfig();

};

