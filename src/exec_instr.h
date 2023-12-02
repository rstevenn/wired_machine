#ifndef __EXEC_INSTR_H__
#define __EXEC_INSTR_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "wired.def.h"
#include "utils/base_log.h"

typedef char (exec_inst_t)(vm_op_t* op, char* vram, uint64_t* regiters, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math); 

typedef struct {
    exec_inst_t* run;
    int set;
} exec_t;

// exec fnct
char exec_scall(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);

char exec_load(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);
char exec_loadi(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);
char exec_loada(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);

char exec_save(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);
char exec_savei(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);
char exec_savea(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);
char exec_saveai(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);

// exec list
extern exec_t exec_list[];
const exec_t* init_exec_list(); 

#endif