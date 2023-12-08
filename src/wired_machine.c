#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "wired.def.h"

#include "exec_instr.h"
#include "th_process_ctx.h"
#include "utils/base_log.h"

#define SANITYZE_MEM 1
#define SANITIZE_MATH 1
#define TH_SWITCH 8

// #undef INFO
// #define INFO(...)

void dump_registers(uint64_t* regs) {
  printf("Regs:\n");
  for (size_t i=0; i<REGISTERS_NB; i++) {
    printf("[%lu] : %" PRIu64 "\n", (unsigned long)i, regs[i]);
  }
  printf("\n");
}

char *readAllFile(char *path, size_t *file_size_out) {
  // open file
  FILE *fp = fopen(path, "rb");
  CHECK_ALLOCATE(fp, "Can't read the file %s", path);

  // get file size
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  *file_size_out = size;

  // read data
  char *buffer = (char *)malloc(sizeof(char) * (size + 1));
  CHECK_ALLOCATE(buffer, "Unable to allocate a buffer of %lu chars",
                 (unsigned long)size)

  size_t got;
  CHECK_READ_WRITE(size, got = fread(buffer, sizeof(char), size, fp),
                   "unable to read the file %s (expected %lu != got %lu)", path,
                   (unsigned long)size, (unsigned long)got);
  buffer[got] = '\0';

  // close file
  fclose(fp);

  return buffer;
}

int main(int argc, char *argv[]) {

  // check args
  if (argc <= 1)
    ERROR("No file pass as arg")

  // read file
  size_t file_size;
  char *rawText = readAllFile(argv[1], &file_size);
  char *current = rawText;
  INFO("LOAD: '%s' %lu bits", argv[1], (unsigned long)file_size)

  // extract and sanitize header metadata
  if (file_size < sizeof(wired_vm_header_t))
    ERROR("the file is to short to contain a header")

  wired_vm_header_t header = *(wired_vm_header_t *)current;
  current += sizeof(header);
  size_t pgm_size = file_size - (current - rawText);

  if (strcmp("WIRE", (const char *)header.identificator) != 0)
    ERROR("Invalid indentificator")

  if (header.ram_size < pgm_size + header.stack_size)
    ERROR("Not enough ram allocated")

  if (header.entry_point > pgm_size)
    ERROR("Entry point out of executable zone");

  // setup ram
  INFO("init ram of %lu bits and load program", (unsigned long)header.ram_size);
  char *vm_ram = (char *)malloc(header.ram_size * sizeof(char));
  if (vm_ram == NULL)
    ERROR("Can't allocate ram memory")

  if (memcpy(vm_ram, current, pgm_size * sizeof(char)) == NULL)
    ERROR("Can't copy program on ram")

  // setup registers
  INFO("setup registers")
  uint64_t registers[REGISTERS_NB] = {0};
  registers[SPL] = header.ram_size - 1;
  registers[SP] = header.ram_size - 1 - header.stack_size;
  registers[PC] = header.entry_point;

  // setup ctx
  INFO("setup context")
  ctx_t ctx;
  ctx.stack_base = &vm_ram[registers[SP]];
  ctx.sp = registers[SP];
  ctx.spl = registers[SPL];

  // setup threads
  INFO("setup threads")
  ths_t *threads = init_ths();
  ths_add_ctx(header.stack_size, registers[PC], ctx.sp, ctx.spl);

  // run
  INFO("Launch programm")
  const exec_t *exec_lst = init_exec_list();

  wm_state_t wm_state = {0};
  wm_state.exit = 0;
  wm_state.pc_set = 0;

  while (!wm_state.exit) {
    // fetch
    INFO("IC: [%lu] fetch at pc: 0x%lx", (unsigned long)registers[IC],
         (unsigned long)registers[PC])
    
    char *pc = vm_ram + registers[PC];
    op_meta_t op_meta = *(op_meta_t *)pc;
    vm_op_t op = {0};
    wm_state.pc_set = 0;

    switch (op_meta.op_size) {
    case (inst_128): {
      INFO("op_size: 128 bits")
      op.meta = op_meta;
      op.args[0] = *(uint64_t *)(pc + sizeof(op_meta));
      break;
    }

    case (inst_192): {
      INFO("op_size: 192 bits")
      op.meta = op_meta;
      op.args[0] = *(uint64_t *)(pc + sizeof(op_meta));
      op.args[1] = *(uint64_t *)(pc + sizeof(op_meta) + sizeof(uint64_t));
      break;
    }

    case (inst_256): {
      INFO("op_size: 256 bits")
      op.meta = op_meta;
      op.args[0] = *(uint64_t *)(pc + sizeof(op_meta));
      op.args[1] = *(uint64_t *)(pc + sizeof(op_meta) + sizeof(uint64_t));
      op.args[2] = *(uint64_t *)(pc + sizeof(op_meta) + 2 * sizeof(uint64_t));
      break;
    }

    default:
      ERROR("Invalide op size '%lu'", (unsigned long)op.meta.op_size)
    }

    // decode && execute
    if (op.meta.op_code >= NB_INST)
      ERROR("Unknow op code 0x%04x", op.meta.op_code)

    if (!exec_lst[op.meta.op_code].set)
      ERROR("op 0x%04x not implemented", op.meta.op_code)

    exec_lst[op.meta.op_code].run(&op, vm_ram, registers, &header, SANITYZE_MEM,
                                  SANITIZE_MATH, &ctx, &wm_state);

    // clock
    registers[IC]++;
    if (!wm_state.pc_set) {
      switch (op_meta.op_size) {
      case (inst_128): {
        registers[PC] += 2 * sizeof(uint64_t);
        break;
      }

      case (inst_192): {
        registers[PC] += 3 * sizeof(uint64_t);
        break;
      }

      case (inst_256): {
        registers[PC] += 4 * sizeof(uint64_t);
        break;
      }

      default:
        ERROR("Invalide op size '%u'", op.meta.op_size)
      }
    }

    // multi thread

    if (wm_state.exit) {
      INFO("Stop thread %lu", (unsigned long)threads->current)
      threads->ths[threads->current].active = 0;
    }

    if (registers[IC] % TH_SWITCH == 0 || wm_state.exit) {
      // find next
      uint64_t old = threads->current;
      uint64_t current = old;

      do {
        current++;

        if (current > threads->nb_ths - 1)
          current = 0;
      } while (!threads->ths[current].active && current != old);

      if (current != old) {
        ths_switch_ctx(current, (registry_t *)registers, ctx.stack_base,
                       header.stack_size);
        INFO("Switch ctx: [%lu] => [%lu]", (unsigned long)old,
             (unsigned long)current)
        wm_state.exit = 0;
      } else {
        INFO("no other thread found")
      }
    }
    dump_registers(registers);
  }

  INFO("exit with code %lu", (unsigned long)registers[RT])

  // clear
  free_ths(); 
  free(vm_ram);
  free(rawText);
  INFO("END")
  return registers[RT];
}
