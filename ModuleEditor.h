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
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
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

