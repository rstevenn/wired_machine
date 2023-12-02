# Description
A project to test user-space threads/processes.
This project provides an assembler and a VM/interpreter for a custom assembly-like language.

# Language synthax and files types
* The language syntax is described in the file *wired_byte_code.txt* .
* Files ending in **.wiasm** contain assembly-like code.
* Files enfing in **.wbin**  contain the assembled binary code.

# Quick start

## linux
Build: 
> ./build.sh 

Run:
> ./run.sh

## windows
Build:
> ./build_wiasm.bat \
> ./build_wired_machine.bat

Run:
> ./bin/wiasm_to_bin.exe  ./test/test.wiasm \ 
> ./bin/wired_machine.exe ./out.wbin

