#pragma once
#include "Module.h"
#include <vector>
#include "UIWindow/DefaultImGuiWindow.h"
struct ImGuiIO;
class ConsoleWindow;
class ConfigurationWindow;
class PreviewWindow;
class HierarchyWindow;
class GameObjectWindow;
class AboutWindow;
class ModuleEditor : public Module
{

public:
	ModuleEditor();
	~ModuleEditor();

	bool Init() override;
	update_status PreUpdate(float deltaTime) override;
	update_status Update(float deltaTime) override;
	update_status PostUpdate(float deltaTime) override;
	bool CleanUp() override;

	void AddWindow(DefaultImGuiWindow *window); //method for adding multiple windows

	ConsoleWindow *consoleWindow;
	ConfigurationWindow *configurationWindow;
	PreviewWindow *previewWindow;
	HierarchyWindow *hierarchyWindow;
	GameObjectWindow *gameObjectWindow;
	AboutWindow *aboutWindow;
private:
	std::vector<DefaultImGuiWindow *> windows;
	void ShowDockSpace(bool *pOpen);
	void DrawMenu();
	void SetStyle(const ImGuiIO& io);
	void UpdateWindows();
	bool showDemoWindow;
	bool showDock;
	update_status appStatus;
};