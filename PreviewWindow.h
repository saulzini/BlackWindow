#pragma once
#include "DefaultImGuiWindow.h"
class PreviewWindow :
    public DefaultImGuiWindow
{
public:
	PreviewWindow(
		const char* _title,
		ImGuiWindowFlags _window_flags
	) : DefaultImGuiWindow(_title, _window_flags) {

	};
	void Update() override;
};

