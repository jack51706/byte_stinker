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

If there's multiple, you'll have to manually remove the second PE header to seperate the first with the second (will be fixed later on)

For more PE reference refer to : [PE Format](https://docs.microsoft.com/en-us/windows/win32/debug/pe-format?redirectedfrom=MSDN)

# Demonstration
![Exampler](https://i.imgur.com/0SCcV1Z.png)

input args : byte_stinker pe_location output_path_name
