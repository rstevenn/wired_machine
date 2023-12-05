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
// macro black magic
#define EXEC_INST_LOOP() \
ACT(exec_scall,  SCALL_INST) \
ACT(exec_load,   LOAD_INST)  \
ACT(exec_loadi,  LOADI_INST) \
ACT(exec_loada,  LOADA_INST) \
ACT(exec_save,   SAVE_INST)  \
ACT(exec_savei,  SAVEI_INST) \
ACT(exec_savea,  SAVEA_INST) \
ACT(exec_saveai, SAVEAI_INST)\
ACT(exec_add,    ADD_INST)   \
ACT(exec_addi,   ADDI_INST)  \
ACT(exec_min,    MIN_INST)   \
ACT(exec_mini,   MINI_INST)  \
ACT(exec_mult,   MULT_INST)  \
ACT(exec_multi,  MULTI_INST) \
ACT(exec_div,    DIV_INST)   \
ACT(exec_divi,   DIVI_INST)  \
ACT(exec_and,    AND_INST)   


// fnct prototypes
#define ACT(fnct, id) char fnct(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math);
EXEC_INST_LOOP()
#undef ACT

// exec list
extern exec_t exec_list[];
const exec_t* init_exec_list(); 

#endif
