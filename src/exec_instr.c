#include "exec_instr.h"

// exec data
exec_t exec_list[NB_INST];

const exec_t *init_exec_list() {
  if (memset(exec_list, 0, NB_INST * sizeof(exec_t)) == NULL)
    ERROR("can't set memory")

// macro black magic
#define ACT(fnct, id) exec_list[id] = (exec_t){(exec_inst_t *)&fnct, 1};
  EXEC_INST_LOOP()
#undef ACT

  return (const exec_t *)exec_list;
}

// exec fn
void exec_scall(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {

  INFO("SCALL 0x%04lx", (unsigned long)op->args[0])
  switch (op->args[0]) {
  case (0): {
    INFO("no op")
    break;
  }

  case (1) : {
    INFO("exit")
    wm_state->exit = 1;
    break;
  }

  default:
    WARNING("Invalid scall number 0x%04lx", (unsigned long)op->args[0])
    break;
  }
  wm_state->pc_set = 0;
}

void exec_load(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("LOAD")
  registers[op->args[0]] = registers[op->args[1]];
  wm_state->pc_set = 0;
}

void exec_loadi(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("LOADI")
  registers[op->args[0]] = op->args[1];
  wm_state->pc_set = 0;
}

void exec_loada(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("LOADA")

  if (sanityse_math) {
    if (op->args[1] < 0 || op->args[1] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)op->args[1])
  }
  registers[op->args[0]] = *(vram + op->args[1]);
  wm_state->exit = 0;
}

void exec_save(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("SAVE")

  if (sanityse_mem == 1) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)op->args[1])
  }
  *(vram + registers[op->args[0]]) = registers[op->args[1]];
  wm_state->pc_set = 0;
}

void exec_savei(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("SAVEI")
  if (sanityse_mem) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)op->args[1])
  }
  *(vram + registers[op->args[0]]) = op->args[1];
  wm_state->pc_set = 0;
}

void exec_savea(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("SAVEA")
  if (sanityse_mem == 1) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)op->args[1])
  }
  *(vram + op->args[0]) = registers[op->args[1]];
  wm_state->pc_set = 0;
}

void exec_saveai(vm_op_t *op, char *vram, uint64_t *registers,
                 wired_vm_header_t *header, char sanityse_mem,
                 char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("SAVEAI")
  if (sanityse_mem) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)op->args[1])
  }
  *(vram + op->args[0]) = op->args[1];
  wm_state->pc_set = 0;
}

void exec_add(vm_op_t *op, char *vram, uint64_t *registers,
              wired_vm_header_t *header, char sanityse_mem,
              char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("ADD")
  registers[op->args[0]] = registers[op->args[1]] + registers[op->args[2]];
  wm_state->pc_set = 0;
}

void exec_addi(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("ADDI")
  registers[op->args[0]] = registers[op->args[1]] + op->args[2];
  wm_state->pc_set = 0;
}

void exec_min(vm_op_t *op, char *vram, uint64_t *registers,
              wired_vm_header_t *header, char sanityse_mem,
              char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("MIN")
  registers[op->args[0]] = registers[op->args[1]] - registers[op->args[2]];
  wm_state->pc_set = 0;
}

void exec_mini(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("MINI")
  registers[op->args[0]] = registers[op->args[1]] - op->args[2];
  wm_state->pc_set = 0;
}

void exec_mult(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("MULT")
  registers[op->args[0]] = registers[op->args[1]] * registers[op->args[2]];
  wm_state->pc_set = 0;
}

void exec_multi(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("MULTI")
  registers[op->args[0]] = registers[op->args[1]] * op->args[2];
  wm_state->pc_set = 0;
}

void exec_div(vm_op_t *op, char *vram, uint64_t *registers,
              wired_vm_header_t *header, char sanityse_mem,
              char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("DIV")
  if (sanityse_math) {
    if (registers[op->args[2]] == 0)
      ERROR("Divide by 0")
  }
  registers[op->args[0]] = registers[op->args[1]] / registers[op->args[2]];
  wm_state->pc_set = 0;
}

void exec_divi(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("DIVI")
  if (sanityse_math == 1) {
    if (op->args[2] == 0)
      ERROR("Divide by 0")
  }
  registers[op->args[0]] = registers[op->args[1]] / op->args[2];
  wm_state->pc_set = 0;
}

void exec_and(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("AND")
    registers[op->args[0]] = registers[op->args[1]] & registers[op->args[2]];
    wm_state->pc_set = 0;
}

void exec_andi(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("ANDI")
    registers[op->args[0]] = registers[op->args[1]] & op->args[2];
    wm_state->pc_set = 0;
}

void exec_or(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("OR")
    registers[op->args[0]] = registers[op->args[1]] | registers[op->args[2]];
    wm_state->pc_set = 0;
}

void exec_ori(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("ORI")
    registers[op->args[0]] = registers[op->args[1]] | op->args[2];  
    wm_state->pc_set = 0;
}

void exec_xor(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("XOR")
    registers[op->args[0]] = registers[op->args[1]] ^ registers[op->args[2]];
    wm_state->pc_set = 0;
}

void exec_xori(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("XORI")
    registers[op->args[0]] = registers[op->args[1]] ^ op->args[2];
    wm_state->pc_set = 0;
}

void exec_not(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("NOT")
    registers[op->args[0]] = ~registers[op->args[1]];
    wm_state->pc_set = 0;
}

void exec_noti(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("NOTI")
    registers[op->args[0]] = ~op->args[1];
    wm_state->pc_set = 0;
}

void exec_eq(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("EQ")
    registers[EQ] = (registers[op->args[0]] == registers[op->args[1]]);
    wm_state->pc_set = 0;
}

void exec_eqi(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("EQI")
    registers[EQ] = (registers[op->args[0]] == op->args[1]);
    wm_state->pc_set = 0;
}

void exec_neq(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("NEQ")
    registers[EQ] = (registers[op->args[0]] != registers[op->args[1]]);
    wm_state->pc_set = 0;
}

void exec_neqi(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("NEQI")
    registers[EQ] = (registers[op->args[0]] != registers[op->args[1]]);
    wm_state->pc_set = 0;
}

void exec_gt(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("GT")
    registers[EQ] = (registers[op->args[0]] > registers[op->args[1]]);
    wm_state->pc_set = 0;
}

void exec_gti(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("GTI")
    registers[EQ] = (registers[op->args[0]] > op->args[1]);
    wm_state->pc_set = 0;
}

void exec_gte(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("GTE")
    registers[EQ] = (registers[op->args[0]] >= registers[op->args[1]]);
    wm_state->pc_set = 0;
}

void exec_gtei(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("GTEI")
    registers[EQ] = (registers[op->args[0]] >= op->args[1]);
    wm_state->pc_set = 0;
}

void exec_lt(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("LT")
    registers[EQ] = (registers[op->args[0]] < registers[op->args[1]]);
    wm_state->pc_set = 0;
}

void exec_lti(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("LTI")
    registers[EQ] = (registers[op->args[0]] < op->args[1]);
    wm_state->pc_set = 0;
}

void exec_lte(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("LTE")
    registers[EQ] = (registers[op->args[0]] <= registers[op->args[1]]);
    wm_state->pc_set = 0;
}

void exec_ltei(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
    INFO("LTEI")
    registers[EQ] = (registers[op->args[0]] <= op->args[1]);
    wm_state->pc_set = 0;
}

void exec_jre(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JRE")
  uint64_t addr = registers[PC] + registers[op->args[0]];
  
  if (sanityse_mem) {
    if (addr < 0 || addr > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  wm_state->pc_set = 1;
}

void exec_jrei(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JRE")
  uint64_t addr = registers[PC] + op->args[0];
  if (sanityse_mem) {
      if (addr < 0 || addr > header->ram_size - 1)
        ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  wm_state->pc_set = 1;
}

void exec_jeq(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JEQ")
  if (!registers[EQ]) {
    wm_state->pc_set = 0;
    return;
  }

  uint64_t addr = registers[op->args[0]];
  if (sanityse_mem) {
    if (addr < 0 || addr > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  wm_state->pc_set = 1;
}

void exec_jeqi(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JEQI")
  if (!registers[EQ]) {
    wm_state->pc_set = 0;
    return;
  }
  uint64_t addr = op->args[0];
  if (sanityse_mem) {
    if (addr < 0 || addr > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  wm_state->pc_set = 1;
}

void exec_jne(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JNE")
  if (registers[EQ]){ 
    wm_state->pc_set = 0;
    return;
  }
  uint64_t addr = registers[op->args[0]];
  if (sanityse_mem) {
    if (addr < 0 || addr > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  wm_state->pc_set = 1;
}

void exec_jnei(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JNEI")
  if (registers[EQ]){ 
    wm_state->pc_set = 0;
    return;
  }
  uint64_t addr = op->args[0];
  if (sanityse_mem){
    if (addr < 0 || addr > header->ram_size - 1)
        ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  wm_state->pc_set = 1;
}

void exec_jmp(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JMP")
  uint64_t addr = registers[op->args[0]];
  
  if (sanityse_mem){
    if (addr < 0 || addr > header->ram_size - 1)
      ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  wm_state->pc_set = 1;
}

void exec_jmpi(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JUMPI")
  uint64_t addr = op->args[0];

  if(sanityse_mem) {
      if (addr < 0 || addr > header->ram_size - 1)
        ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  registers[PC] = addr;
  //exit(1); // only useful for tests
  wm_state->pc_set = 1;
}

void exec_jmpth(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JMP_TH")
  uint64_t addr = registers[op->args[0]];

  if (sanityse_mem) {
      if (addr < 0 || addr > header->ram_size - 1)
        ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  ths_add_ctx(header->stack_size, registers[op->args[0]], ctx->sp, ctx->spl);
  wm_state->pc_set = 0;
}

void exec_jmpthi(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math, ctx_t* ctx, wm_state_t* wm_state) {
  INFO("JUMPTHI")
  uint64_t addr = op->args[0];
  if (sanityse_mem) {
      if (addr < 0 || addr > header->ram_size - 1)
        ERROR("Out of range adress 0x%04lx", (unsigned long)addr)
  }
  ths_add_ctx(header->stack_size, op->args[0], ctx->sp, ctx->spl);
  wm_state->pc_set = 0;
}