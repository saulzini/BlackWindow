#include "DefaultImGuiWindow.h"

DefaultImGuiWindow::~DefaultImGuiWindow()
{
}

bool DefaultImGuiWindow::begin()
{
	return ImGui::Begin(title, &show, window_flags);
}

void DefaultImGuiWindow::end()
{
	ImGui::End();
}
