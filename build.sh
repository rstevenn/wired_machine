#!/bin/sh
set -ex
gcc -o2 -Wall -Werror -g src/parse_wiasm.c src/wiasm_to_wbin.c -o bin/wiasm_to_wbin
gcc -o2 -Wall -Werror -g src/wired_machine.c src/exec_instr.c src/th_process_ctx.c -o bin/wired_machine

