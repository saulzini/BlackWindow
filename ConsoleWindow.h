#pragma once

#include "DefaultImGuiWindow.h"

class ConsoleWindow :
	public DefaultImGuiWindow
{
public:
	ConsoleWindow(const char* _title) : DefaultImGuiWindow(_title) {
	}
	void update() override;
};

