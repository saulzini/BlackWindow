#pragma once
#include "DefaultImGuiWindow.h"
class HierarchyWindow :
    public DefaultImGuiWindow
{
public:
    HierarchyWindow(
		const char* _title,
		ImGuiWindowFlags _window_flags
	) : DefaultImGuiWindow(_title, _window_flags) {

	};
	void Update() override;
};

