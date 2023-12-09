# Description
A project to test user-space threads/processes.
This project provides an assembler and a VM/interpreter for a custom assembly-like language.

# Language synthax and files types
* The language syntax is described in the file *wired_byte_code.txt* .
* Files ending in **.wiasm** contain assembly-like code.
* Files enfing in **.wbin**  contain the assembled binary code.

# Dependency 
* gcc

# Quick start

## linux
### Build: 
> ./build.sh 

### Run Test:
> ./run.sh

### Run custom programm
compile to wbin :
> ./bin/wiasm_to_bin < path to your .wiasm file > 

run :
> ./bin/wired_machine < path to the out.wbin file > 

## windows
### Build:
> ./build_wiasm.bat \
> ./build_wired_machine.bat

### Run Test:
> ./bin/wiasm_to_wbin.exe  ./test/test.wiasm \
> ./bin/wired_machine.exe ./out.wbin

### Run custom programm
compile to wbin :
> ./bin/wiasm_to_wbin.exe < path to your .wiasm file > 

run :
> ./bin/wired_machine.exe < path to the out.wbin file > 

# TODO
* [x] check for memory leaks
* [x] use https://github.com/rstevenn/ccbase for logging
* [x] implement a string view library to improve parssing
* [ ] refactor parssing
* [ ] implement .code and .data sections