# byte_stinker
A PoC PE dumper designed to pull (potentially malicious) extra binaries stored in programs.

# Information

This program will do the following :
- Loop through the entire size of the process
- Look for "MZ" occurences
- Verify it's not the main program (the first MZ header)
- Copy the PE file to a desktop location. 

# Extra
Please note that this assumes that all binaries are .exe's, and supports x64 binaries only. (should be easy to implement within 3 minutes)

If the binary is packed you'll need to dump the process's memory before trying to use byte_stinker

It assumes that the binary is x64 (will release a update later to detect what type of file it is)

For more PE reference refer to : [PE Format](https://docs.microsoft.com/en-us/windows/win32/debug/pe-format?redirectedfrom=MSDN)

# Demonstration
![Exampler](https://i.imgur.com/0SCcV1Z.png)

input args : byte_stinker pe_location output_path_name

Disclaimer : I do not plan on updating this anymore. I may plan on creating a seperate branch for runtime based scans through all modules of a specified process, but only If I have time. You can easily distinguish what is a DLL/EXE/System File by either it's imports, PE characteristics (IMAGE_FILE_DLL, IMAGE_FILE_SYSTEM, etc.).
