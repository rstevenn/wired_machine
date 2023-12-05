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
char exec_scall(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math) {

  INFO("SCALL 0x%04ldx", op->args[0])
  switch (op->args[0]) {
  case (0): {
    INFO("no op")
    break;
  }

  default:
    WARNING("Invalid scall number 0x%04ldx", op->args[0])
    break;
  }
  return 0;
}

char exec_load(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math) {
  INFO("LOAD")
  registers[op->args[0]] = registers[op->args[1]];
  return 0;
}

char exec_loadi(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math) {
  INFO("LOADI")
  registers[op->args[0]] = op->args[1];
  return 0;
}

char exec_loada(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math) {
  INFO("LOADA")

  if (sanityse_math) {
    if (op->args[1] < 0 || op->args[1] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04ldx", op->args[1])
  }
  registers[op->args[0]] = *(vram + op->args[1]);
  return 0;
}

char exec_save(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math) {
  INFO("SAVE")

  if (sanityse_mem == 1) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04ldx", op->args[1])
  }
  *(vram + registers[op->args[0]]) = registers[op->args[1]];
  return 0;
}

char exec_savei(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math) {
  INFO("SAVEI")
  if (sanityse_mem) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04ldx", op->args[1])
  }
  *(vram + registers[op->args[0]]) = op->args[1];
  return 0;
}

char exec_savea(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math) {
  INFO("SAVEA")
  if (sanityse_mem == 1) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04ldx", op->args[1])
  }
  *(vram + op->args[0]) = registers[op->args[1]];
  return 0;
}

char exec_saveai(vm_op_t *op, char *vram, uint64_t *registers,
                 wired_vm_header_t *header, char sanityse_mem,
                 char sanityse_math) {
  INFO("SAVEAI")
  if (sanityse_mem) {
    if (op->args[0] < 0 || op->args[0] > header->ram_size - 1)
      ERROR("Out of range adress 0x%04ldx", op->args[1])
  }
  *(vram + op->args[0]) = op->args[1];
  return 0;
}

char exec_add(vm_op_t *op, char *vram, uint64_t *registers,
              wired_vm_header_t *header, char sanityse_mem,
              char sanityse_math) {
  INFO("ADD")
  registers[op->args[0]] = registers[op->args[1]] + registers[op->args[2]];
  return 0;
}

char exec_addi(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math) {
  INFO("ADDI")
  registers[op->args[0]] = registers[op->args[1]] + op->args[2];
  return 0;
}

char exec_min(vm_op_t *op, char *vram, uint64_t *registers,
              wired_vm_header_t *header, char sanityse_mem,
              char sanityse_math) {
  INFO("MIN")
  registers[op->args[0]] = registers[op->args[1]] - registers[op->args[2]];
  return 0;
}

char exec_mini(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math) {
  INFO("MINI")
  registers[op->args[0]] = registers[op->args[1]] - op->args[2];
  return 0;
}

char exec_mult(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math) {
  INFO("MULT")
  registers[op->args[0]] = registers[op->args[1]] * registers[op->args[2]];
  return 0;
}

char exec_multi(vm_op_t *op, char *vram, uint64_t *registers,
                wired_vm_header_t *header, char sanityse_mem,
                char sanityse_math) {
  INFO("MULTI")
  registers[op->args[0]] = registers[op->args[1]] * op->args[2];
  return 0;
}

char exec_div(vm_op_t *op, char *vram, uint64_t *registers,
              wired_vm_header_t *header, char sanityse_mem,
              char sanityse_math) {
  INFO("DIV")
  if (sanityse_math) {
    if (registers[op->args[2]] == 0)
      ERROR("Divide by 0")
  }
  registers[op->args[0]] = registers[op->args[1]] / registers[op->args[2]];
  return 0;
}

char exec_divi(vm_op_t *op, char *vram, uint64_t *registers,
               wired_vm_header_t *header, char sanityse_mem,
               char sanityse_math) {
  INFO("DIVI")
  if (sanityse_math == 1) {
    if (op->args[2] == 0)
      ERROR("Divide by 0")
  }
  registers[op->args[0]] = registers[op->args[1]] / op->args[2];
  return 0;
}

char exec_and(vm_op_t *op, char *vram, uint64_t *registers, wired_vm_header_t *header, char sanityse_mem, char sanityse_math) {
    INFO("AND")
    registers[op->args[0]] = registers[op->args[1]] & registers[op->args[2]];
    return 0;
}



