#pragma once
#include "Module.h"
#include <vector>
#include "DefaultImGuiWindow.h"


struct ImGuiIO;
class ConsoleWindow;
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
private:
	std::vector<DefaultImGuiWindow*> windows;
	void ShowDockSpace(bool* p_open);
	void SetStyle(const ImGuiIO io);
	void UpdateWindows();

	void PressGuiBtn();
	void PressAboutBtn();
};

