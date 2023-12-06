#include "th_process_ctx.h"

// static thx context
ths_t *threads_ctx = NULL;

ths_t* init_ths() {
  threads_ctx = (ths_t *)malloc(sizeof(ths_t));
  if (threads_ctx == NULL)
    ERROR("can't allocate memory for thes table")

  threads_ctx->ths = NULL;
  threads_ctx->nb_ths = 0;
  threads_ctx->allocated_ths = 0;
  threads_ctx->current = 0;

  return  threads_ctx;
}

void ths_add_ctx(uint64_t stack_size, uint64_t pc, uint64_t sp,
             uint64_t spl) {

  threads_ctx->nb_ths++;
  if (threads_ctx->nb_ths > threads_ctx->allocated_ths) {
    if (threads_ctx->allocated_ths == 0)
      threads_ctx->allocated_ths = 1;

    threads_ctx->ths = (vm_th_t *)realloc(
        threads_ctx->ths, sizeof(vm_th_t) * threads_ctx->allocated_ths * 2);
    threads_ctx->allocated_ths *= 2;
    if (threads_ctx->ths == NULL)
      ERROR("Can't allocate threads")
  }

  threads_ctx->ths[threads_ctx->nb_ths - 1].active = 1;
  char *out = memset(threads_ctx->ths[threads_ctx->nb_ths - 1].regiters, 0,
                     sizeof(uint64_t) * REGISTERS_NB);
  if (out == NULL)
    ERROR("Can't set registers")

  threads_ctx->ths[threads_ctx->nb_ths - 1].regiters[PC] = pc;
  threads_ctx->ths[threads_ctx->nb_ths - 1].regiters[SP] = sp;
  threads_ctx->ths[threads_ctx->nb_ths - 1].regiters[SPL] = spl;

  threads_ctx->ths[threads_ctx->nb_ths - 1].stack = (char *)malloc(stack_size);
  if (threads_ctx->ths[threads_ctx->nb_ths - 1].stack == NULL)
    ERROR("Can't allocate stack")
}

void ths_switch_ctx(uint64_t th_id, registry_t *hr,
                char *stack_base, size_t stack_size) {
  // store
  vm_th_t *c_thread = &threads_ctx->ths[threads_ctx->current];
  if (memcpy(c_thread->regiters, hr, REGISTERS_NB) == NULL)
    ERROR("Can't copy registers")
  if (memcpy(c_thread->stack, stack_base, stack_size) == NULL)
    ERROR("Can't copy vram")

  // load
  c_thread = &threads_ctx->ths[th_id];
  if (memcpy(hr, c_thread->regiters, REGISTERS_NB) == NULL)
    ERROR("Can't copy registers")
  if (memcpy(stack_base, c_thread->stack, stack_size) == NULL)
    ERROR("Can't copy vram")

  threads_ctx->current = th_id;
}

