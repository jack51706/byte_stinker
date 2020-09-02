#pragma once
#include "output.h"

namespace output
{
	std::string text_format(const char* text, ...)
	{
		va_list args;
		va_start(args, text);

		char buffer[2048];
		vsprintf_s(buffer, text, args);

		va_end(args);

		return buffer;
	}

	void debug_print(bool bIsTrue, const char* text, ...)
	{
		va_list args;
		va_start(args, text);

		char buffer[2048];
		vsprintf_s(buffer, text, args);

		va_end(args);

		switch (bIsTrue)
		{
		case 1:
			printf("[+] %s\n", buffer);
			break;
		default:
			printf("[-] %s\n", buffer);
			break;
		}

		return;
	}
}