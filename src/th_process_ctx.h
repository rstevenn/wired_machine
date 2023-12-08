#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "wired.def.h"

#ifndef __PROCESS_TH_CTX_H__
#define __PROCESS_TH_CTX_H__

// threads ctx
typedef struct {
  uint64_t regiters[REGISTERS_NB];
  char *stack;
  char active;
} vm_th_t;

typedef struct {
  vm_th_t *ths;
  uint64_t nb_ths;
  uint64_t allocated_ths;
  uint64_t current;
} ths_t;

typedef struct {
  char *stack_base;
  uint64_t sp;
  uint64_t spl;
} ctx_t;


// api
ths_t* init_ths();
void ths_add_ctx(uint64_t stack_size, uint64_t pc, uint64_t sp, uint64_t spl);
void ths_switch_ctx(uint64_t th_id, registry_t *hr, char *stack_base, size_t stack_size);
void free_ths();

#endif
