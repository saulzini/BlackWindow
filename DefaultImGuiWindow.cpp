#include "DefaultImGuiWindow.h"

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



bool DefaultImGuiWindow::begin()
{
	return ImGui::Begin(title, &show, window_flags);
}

void DefaultImGuiWindow::end()
{
	ImGui::End();
}
