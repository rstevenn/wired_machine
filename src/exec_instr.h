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
#define EXEC_INST_LOOP() \
ACT(exec_scall)  \
ACT(exec_load)   \
ACT(exec_loadi)  \
ACT(exec_loada)  \
ACT(exec_save)   \
ACT(exec_savei)  \
ACT(exec_savea)  \
ACT(exec_saveai) \
ACT(exec_add)    \
ACT(exec_addi)   \
ACT(exec_min)    \
ACT(exec_mini)   \
ACT(exec_mult)   \
ACT(exec_multi)  \
ACT(exec_div)    \
ACT(exec_divi)   


#define ACT(fnct) char fnct(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);
EXEC_INST_LOOP()
#undef ACT

// exec list
extern exec_t exec_list[];
const exec_t* init_exec_list(); 

#endif