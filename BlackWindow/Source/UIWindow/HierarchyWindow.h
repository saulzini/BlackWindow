#pragma once
#include "DefaultImGuiWindow.h"
class GameObject;
class HierarchyWindow :
    public DefaultImGuiWindow
{
public:
    HierarchyWindow(
		const char* _title,
		ImGuiWindowFlags _window_flags
	) : DefaultImGuiWindow(_title, _window_flags) {
		originalNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	};
	void Update() override;
	void TreeChildren(ImGuiTreeNodeFlags node_flags, bool isOpen,GameObject *currentNode);

protected:
	GameObject *selected;
	ImGuiTreeNodeFlags originalNodeFlags;

};

