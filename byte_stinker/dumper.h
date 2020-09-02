#pragma once
#include "stdafx.h"

namespace dumper
{
	bool dump_stored_binary(const std::string& pe_path, const std::string& stored_location);
	bool is_valid_pe(PVOID image);
	PVOID convert_to_bytes(const std::string& pe_path, size_t* size);
	bool byte_to_disk(const std::string& stored_location, const char* bytes, size_t size);
	int is_last_pe(std::uint8_t* image, size_t size);
}