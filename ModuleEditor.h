#pragma once
#include "Module.h"
#include <vector>
#include "DefaultImGuiWindow.h"


struct ImGuiIO;
class ConsoleWindow;
class ConfigurationWindow;
class ModuleEditor :
	public Module
{

public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void addWindow(DefaultImGuiWindow *window); //method for adding multiple windows

	ConsoleWindow* consoleWindow;
	ConfigurationWindow* configurationWindow;
private:
	std::vector<DefaultImGuiWindow*> windows;
	void ShowDockSpace(bool* pOpen);
	void SetStyle(const ImGuiIO io);
	void UpdateWindows();

};

