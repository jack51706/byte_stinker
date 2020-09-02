#pragma once
#include "dumper.h"

namespace dumper
{
	bool is_valid_pe(PVOID image)
	{
		const auto dos_header = (PIMAGE_DOS_HEADER)(image); /*the start of the PE header.*/
		if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
		{
			output::debug_print(false, "Invalid DOS signature");
			return false;
		}

		const auto nt_header = (PIMAGE_NT_HEADERS64)((uint64_t)image + dos_header->e_lfanew);
		if (nt_header->Signature != IMAGE_NT_SIGNATURE)
		{
			output::debug_print(false, "Invalid NtHeader signature.");
			return false;
		}

		return true;
	}

	PVOID convert_to_bytes(const std::string& pe_path, size_t* size)
	{
		HANDLE mapped_object, file = 0;
		PVOID image = 0;

		file = CreateFileA(pe_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			output::debug_print(false, "Failed to find the PE's location, are you sure you entered the correct location?, Error Code : %d", GetLastError());
			return { 0 };
		}
		
		mapped_object = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, 0, NULL);
		if (mapped_object == INVALID_HANDLE_VALUE)
		{
			output::debug_print(false, "Failed to map Create File Mapping. Error Code : %d", GetLastError());
			return { 0 };
		}

		image = MapViewOfFile(mapped_object, FILE_MAP_READ, 0, 0, 0);

		if (is_valid_pe(image))
		{
			*size = GetFileSize(file, NULL);
			return image;
		}

		output::debug_print(false, "Failed to MapViewOfFile! Error Code : %d", GetLastError());

		return { 0 };
	}

	bool dump_stored_binary(const std::string& pe_path, const std::string& stored_location)
	{
		int last_pe = 0;
		int dumped = 0;
		size_t size = 0;
		std::vector<std::uint8_t*> pe_files;

		auto pe_header = (std::uint8_t*)convert_to_bytes(pe_path, &size); // get the size of the program to loop.

		if (pe_header != nullptr)
		{
			if (is_valid_pe(pe_header))
			{
				for (auto i = 0; i < size; i++)
				{
					// MZ (dos-stub)
					// should be cleaned up or perferrably look for something more reliable (ex. dos-stub).
					if (pe_header[i] == 0x4D && pe_header[i + 1] == 0x5A && pe_header[i + 2] == 0x90 && pe_header[i + 3] == 0x00 && pe_header[i + 4] == 0x03)
					{
						pe_files.push_back(pe_header + i);

						for (auto header : pe_files)
						{
							if (i != 0 && last_pe != i) // make's sure that it's not the first MZ header (the real program) and isn't repeating the the same PE header.
							{
								if (is_valid_pe(header))
								{
									auto dos_header = (PIMAGE_DOS_HEADER)(header + i);
									auto ntheader = (PIMAGE_NT_HEADERS64)((uint64_t)(header + i) + dos_header->e_lfanew);
									output::debug_print(true, "Found MZ header! 0x%X", i);
									output::debug_print(true, "Writing binary to given location....");

									std::string output = stored_location + output::text_format("_%i.exe", dumped); // assumes EXE (figure it yourseslf within the dissasembly or else.)


									auto end_pe = is_last_pe(header + i, size - i);
									if (end_pe)
									{
										byte_to_disk(output, (const char*)dos_header, end_pe); 
									}
									else
									{
										output::debug_print(false, "Last PE detected!");
										byte_to_disk(output, (const char*)dos_header, size - i); 
									}
								}

								last_pe = i; // to only get the MZ header location once (of the same PE)
								dumped++;
							}
						}
					}
				}

				// verify that it did actually find PE files to dump.
				if (pe_files.size())
				{
					output::debug_print(true, "Dumped %d PE(s)!", dumped);
					return true;
				}
				
				output::debug_print(false, "Failed to find PE files embedded in program. If it's packed please unpack before using byte_stinker (dump PE)");
				return false;
			}

			output::debug_print(false, "Not a VALID PE header!");
		}

		output::debug_print(false, "Failed to convert program to byte array.");
		return false;
	}

	int is_last_pe(std::uint8_t* image, size_t size)
	{

		for (auto i = 0; i < size; i++)
		{
			if (image[i] == 0x4D && image[i + 1] == 0x5A && image[i + 2] == 0x90 && image[i + 3] == 0x00 && image[i + 4] == 0x03 && i != 0)
			{
				return i;
			}
		}

		return 0;
	}

	bool byte_to_disk(const std::string& stored_location, const char* bytes, size_t size)
	{
		std::ofstream stream(stored_location.c_str(), std::ios_base::out | std::ios_base::binary);

		if (!stream.write(bytes, size))
		{
			stream.close();
			return false;
		}

		stream.close();
		return true;
	}
}