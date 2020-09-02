#pragma once
#include "stdafx.h"

namespace output
{
	void debug_print(bool bIsTrue, const char* text, ...);
	std::string text_format(const char* text, ...);
}