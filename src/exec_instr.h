#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "wired.def.h"
#include "th_process_ctx.h"
#include "utils/base_log.h"

#ifndef __EXEC_INSTR_H__
#define __EXEC_INSTR_H__

typedef void (exec_inst_t)(vm_op_t* op, char* vram, uint64_t* regiters, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math, ctx_t* ctx, wm_state_t* wm_state); 

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
ACT(exec_and,    AND_INST)   \
ACT(exec_andi,   ANDI_INST)  \
ACT(exec_or,     OR_INST)    \
ACT(exec_ori,    ORI_INST)   \
ACT(exec_xor,    XOR_INST)   \
ACT(exec_xori,   XORI_INST)  \
ACT(exec_not,    NOT_INST)   \
ACT(exec_noti,   NOTI_INST)  \
ACT(exec_eq,     EQ_INST)    \
ACT(exec_eqi,    EQI_INST)   \
ACT(exec_neq,    NEQ_INST)   \
ACT(exec_neqi,   NEQI_INST)  \
ACT(exec_gt,     GT_INST)    \
ACT(exec_gti,    GTI_INST)   \
ACT(exec_gte,    GTE_INST)   \
ACT(exec_gtei,   GTEI_INST)  \
ACT(exec_lt,     LT_INST)    \
ACT(exec_lti,    LTI_INST)   \
ACT(exec_lte,    LTE_INST)   \
ACT(exec_ltei,   LTEI_INST)  \
ACT(exec_jre,    JRE_INST)   \
ACT(exec_jrei,   JREI_INST)  \
ACT(exec_jeq,    JEQ_INST)   \
ACT(exec_jeqi,   JEQI_INST)  \
ACT(exec_jne,    JNE_INST)   \
ACT(exec_jnei,   JNEI_INST)  \
ACT(exec_jmp,    JMP_INST)   \
ACT(exec_jmpi,   JMPI_INST)  \
ACT(exec_jmpth,  JMP_TH_INST)\
ACT(exec_jmpthi, JMP_THI_INST)  

// fnct prototypes
#define ACT(fnct, id) void fnct(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math, ctx_t* ctx, wm_state_t* wm_state);
EXEC_INST_LOOP()
#undef ACT

// exec list
extern exec_t exec_list[];
const exec_t* init_exec_list(); 

#endif
