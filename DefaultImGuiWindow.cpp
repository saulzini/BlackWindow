#include "DefaultImGuiWindow.h"
#include "ImGui/imgui.h"
DefaultImGuiWindow::~DefaultImGuiWindow()
{
}
/*
DefaultImGuiWindow::DefaultImGuiWindow(std::string _title)
{
	show = true;
	title = _title;
}
*/
void DefaultImGuiWindow::update()
{
}


bool DefaultImGuiWindow::begin()
{
	return ImGui::Begin(title, &show);
}

void DefaultImGuiWindow::end()
{
	ImGui::End();
}
