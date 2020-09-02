#pragma once
#include "stdafx.h"

int main(int argc, char** argv)
{
	SetConsoleTitleA("byte_stinker | beta v1.1");

	if (argc < 3)
	{
		output::debug_print(false, "byte_stinker - incorrect arguments.\n[+] args are : byte_stinker.exe exe_path.exe output_path");
		return -1;
	}

	output::debug_print(true, "Loaded byte_stinker! [BUILD v1.1]");

	dumper::dump_stored_binary(argv[1], argv[2]);
}