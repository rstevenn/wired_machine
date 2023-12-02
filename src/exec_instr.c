#include "exec_instr.h"

// exec data
exec_t exec_list[NB_INST];

const exec_t* init_exec_list() {
    if (memset(exec_list, 0, NB_INST) == NULL)
        ERROR("can't set memory")

    exec_list[SCALL_INST]  = (exec_t){(exec_inst_t*)&exec_scall, 1};
    exec_list[LOAD_INST]   = (exec_t){(exec_inst_t*)&exec_load, 1};
    exec_list[LOADA_INST]  = (exec_t){(exec_inst_t*)&exec_loada, 1};
    exec_list[SAVE_INST]   = (exec_t){(exec_inst_t*)&exec_save, 1};
    exec_list[SAVEI_INST]  = (exec_t){(exec_inst_t*)&exec_savei, 1};
    exec_list[SAVEA_INST]  = (exec_t){(exec_inst_t*)&exec_savea, 1};
    exec_list[SAVEAI_INST] = (exec_t){(exec_inst_t*)&exec_saveai, 1};
    return (const exec_t*)exec_list;
}


// exec fn
char exec_scall(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math) {

    INFO("SCALL 0x%04llx", op->args[0])
    switch (op->args[0]) {
    case (0): {
        INFO("no op")
        break;
    }

    default:
        WARNING("Invalid scall number 0x%04llx", op->args[0])
        break;
    }
    return 0;
}

char exec_load(vm_op_t* op, char* vram, uint64_t* registers,  wired_vm_header_t* header, char sanityse_mem,  char sanityse_math){ 
    INFO("LOAD")
    registers[op->args[0]] = registers[op->args[1]];
    return 0;
}

char exec_loadi(vm_op_t* op, char* vram, uint64_t* registers,  wired_vm_header_t* header, char sanityse_mem,  char sanityse_math){ 
    INFO("LOADI")
    registers[op->args[0]] = op->args[1];
    return 0;
}

char exec_loada(vm_op_t* op, char* vram, uint64_t* registers,  wired_vm_header_t* header,   char sanityse_mem,  char sanityse_math) {
    INFO("LOADA")
    
    if (sanityse_math) {
        if (op->args[1] < 0 || op->args[1] > header->ram_size-1)
          ERROR("Out of range adress 0x%04llx", op->args[1])
    }
    registers[op->args[0]] = *(vram+op->args[1]);
    return 0;
}

char exec_save(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math) {
    INFO("SAVE")

    if (sanityse_mem == 1) {
        if (op->args[0] < 0 || op->args[0] > header->ram_size-1)
          ERROR("Out of range adress 0x%04llx", op->args[1])
    }
    *(vram+registers[op->args[0]]) = registers[op->args[1]];
    return 0;
}

char exec_savei(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math) {
    INFO("SAVEI")
    if (sanityse_mem) {
        if (op->args[0] < 0 || op->args[0] > header->ram_size-1)
          ERROR("Out of range adress 0x%04llx", op->args[1])
    }
    *(vram+registers[op->args[0]]) = op->args[1];
    return 0;
}

char exec_savea(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math) {
    INFO("SAVEA")
    if (sanityse_mem == 1) {
        if (op->args[0] < 0 || op->args[0] > header->ram_size-1)
          ERROR("Out of range adress 0x%04llx", op->args[1])
    }
    *(vram+op->args[0]) = registers[op->args[1]];
    return 0;
}

char exec_saveai(vm_op_t* op, char* vram, uint64_t* registers, wired_vm_header_t* header, char sanityse_mem,  char sanityse_math) {
    INFO("SAVEAI")
    if (sanityse_mem){
        if (op->args[0] < 0 || op->args[0] > header->ram_size-1)
          ERROR("Out of range adress 0x%04llx", op->args[1])
    }
    *(vram+op->args[0]) = op->args[1];
    return 0;
}