#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "wired.def.h"

#define __HASH_MAP_IMPL__
#include "utils/hash_map.h"
#include "parse_wiasm.h"
#include "ccbase/logs/log.h"

#define __CCB_VS_IMP__
#include "ccbase/vs/view_string.h"


void render_pgm(operation_t *op, size_t *size_out, char *data_out) {

  switch (op->size) {
  case inst_128: {
    raw_128_op render_op;
    render_op.op_code = op->type;
    render_op.op_size = inst_128;
    render_op.args.arg0 = op->args.arg_64.arg0;

    *size_out = sizeof(raw_128_op);
    CCB_CHECK(memcpy(data_out, &render_op, sizeof(raw_128_op)) != NULL,
                   "can't copy instruction data");
    break;
  }

  case inst_192: {
    raw_192_op render_op;
    render_op.op_code = op->type;
    render_op.op_size = inst_192;
    render_op.args.arg0 = op->args.arg_128.arg0;
    render_op.args.arg1 = op->args.arg_128.arg1;

    *size_out = sizeof(raw_192_op);
    CCB_CHECK(memcpy(data_out, &render_op, sizeof(raw_192_op)) != NULL,
                   "can't copy instruction data");
    break;
  }

  case inst_256: {
    raw_256_op render_op;
    render_op.op_code = op->type;
    render_op.op_size = inst_256;
    render_op.args.arg0 = op->args.arg_192.arg0;
    render_op.args.arg1 = op->args.arg_192.arg1;
    render_op.args.arg2 = op->args.arg_192.arg2;

    *size_out = sizeof(raw_256_op);
    CCB_CHECK(memcpy(data_out, &render_op, sizeof(raw_256_op)) != NULL,
              "can't copy instruction data");
    break;
  }

  case inst_jmp: {

    raw_128_op render_op;
    hash_map_t *map = find_hash_label(op->args.flag);

    if (map == NULL)
      CCB_ERROR("can't find lable '%s'", op->args.flag)

    render_op.op_code = op->type;
    render_op.op_size = inst_128;
    render_op.args.arg0 = find_hash_label(op->args.flag)->addr;

    *size_out = sizeof(raw_128_op);
    CCB_CHECK(memcpy(data_out, &render_op, sizeof(raw_128_op)) != NULL,
                   "can't copy instruction data");

    break;
  }

  default:
    CCB_ERROR("Unknow instruction with type %u", op->size)
    break;
  }
}

char *readAllFile(char *path) {
  // open file
  FILE *fp = fopen(path, "rb");
  CCB_CHECK(fp != NULL, "Can't read the file %s", path);

  // get file size
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // read data
  char *buffer = (char *)malloc(sizeof(char) * (size + 1));
  CCB_CHECK(buffer != NULL, "Unable to allocate a buffer of %lu chars", (unsigned long)size)

  size_t got = fread(buffer, sizeof(char), size, fp);
  CCB_CHECK(size == got, "unable to read the file %s (expected %lu != got %lu)",
                   path, (unsigned long)size, (unsigned long)got);
  buffer[got] = '\0';

  // close file
  fclose(fp);

  return buffer;
}

int main(int argc, char *argv[]) {
  // init log
  ccb_InitLog("wiasm_to_wbin.log");  

  // check args
  if (argc <= 1)
    CCB_ERROR("No file pass as arg")

  // read file
  char *rawText = readAllFile(argv[1]);
  CCB_INFO("raw txt:\n\n%s\n", rawText);

  wired_vm_header_t header = {0};
  header.identificator[0] = 'W';
  header.identificator[1] = 'I';
  header.identificator[2] = 'R';
  header.identificator[3] = 'E';
  header.identificator[4] = '\0';

  // stack and ram size
  size_t ok;
  ok = sscanf(rawText, "ram_size:%" SCNu64 "\n", &(header.ram_size));
  if (!ok) {
    CCB_ERROR("Invalid ram size or ram size not privided")
  }

  char *next = strpbrk(rawText, "\n");
  if (next == NULL) {
    CCB_ERROR("Stack not privided")
  }

  ok = sscanf(next + 1, "stack_size:%" SCNu64 "\n", &(header.stack_size));
  if (!ok) {
    CCB_ERROR("Invalid stack size or ram size not privided")
  }

  next = strpbrk(next + 1, "\n");
  if (next == NULL) {
    CCB_ERROR("no code found")
  }
  next++;

  // instructions and build label table
  ccb_vs vst_program = ccb_cst2vst((const char *)next);
  printf(CCB_VS_FMT "\n", CCB_VS_ARG(vst_program));

  char buffer_base[64] = {0};
  char *buffer_current = buffer_base;

  // translater state
  char in_instruction = 0;
  size_t nb_args = 0;
  long int instruction_addr = 0;

  program_t program = {0};

  char *current = next;
  char found_entry = 0;
  int i = 0;

  int found = 0;
  do {
    ccb_vs_split split = ccb_vs_split_char(vst_program, '\n');
    found = split.found;
    vst_program = split.back;

    // check for label
    if (ccb_vs_in(split.front, ccb_cst2vst(":"))) {
      ccb_vs current_vs = split.front;

      while (ccb_vs_is_head(current_vs, ccb_cst2vst(" ")) ||
             ccb_vs_is_head(current_vs, ccb_cst2vst("\t"))){
        current_vs = ccb_vs_triml(current_vs, 1);
      }
    }

  } while(found);
  
  while (current != NULL) {

    // CCB_INFO("%d", i)
    i++;

    switch (*current) {

    // comments
    case '#':
      while (*current != '\0' && *current != '\n')
        current++;

      if (*current == '\0')
        current--;

      buffer_current = buffer_base;
      in_instruction = 0;
      nb_args = 0;

      break;


    // split chars
    case ' ':
    case '\t':
    case '\r':
    case '\0':
      if (buffer_base == buffer_current)
        break;

      buffer_current = buffer_base;
      if (!in_instruction) {
        instruction_addr += parse_instruction(buffer_base, &program);
        in_instruction = 1;
        nb_args = 0;

      } else {
        (*program.operations[program.len - 1].parse_args)(
            buffer_base, nb_args, &program.operations[program.len - 1]);
        nb_args++;
      }

      break;

    case '\n': {
      if (buffer_base == buffer_current) {
          buffer_current = buffer_base;
          in_instruction = 0;
          nb_args = 0;
          break;
      }

      if (!in_instruction) {
        instruction_addr += parse_instruction(buffer_base, &program);
      } else {
        (*program.operations[program.len - 1].parse_args)(
            buffer_base, nb_args, &program.operations[program.len - 1]);
        nb_args++;
      }

      buffer_current = buffer_base;
      in_instruction = 0;
      nb_args = 0;
      break;
    }

    // lable
    case ':':
      *buffer_current = '\0';
      table_add_label(buffer_base);
      table_add_adrr(buffer_base, instruction_addr);
      buffer_current = buffer_base;

      // add entry point
      if (strcmp(buffer_base, "ENTRY") == 0) {
        header.entry_point = instruction_addr;
        found_entry = 1;
      }

      break;

    default:
      if (buffer_current - buffer_base > 63)
        CCB_ERROR("Token bigger than 64 bits")

      *buffer_current = *current;
      buffer_current++;
    }

    if (*current == '\0') {
      current = NULL;
      continue;
    }

    *buffer_current = '\0';
    current = (current == NULL) ? NULL : current + 1;
  }

  // check entry
  if (!found_entry)
    CCB_ERROR("did not find the entry point, please set a 'ENTRY:' lable")

  // write program
  FILE *fp = fopen("out.wbin", "wb");
  if (fp == NULL)
    CCB_ERROR("can't open/create the output file")

  if (1 != fwrite(&header, sizeof(header), 1, fp))
    CCB_ERROR("can't write header")

  for (size_t i = 0; i < program.len; i++) {
    size_t size_out = 0;
    char data_out[1024];
    operation_t op = program.operations[i];

    render_pgm(&op, &size_out, (char *)&data_out);

    if (1 != fwrite(data_out, size_out, 1, fp))
      CCB_ERROR("can't wriet instructions")
  }

  // free resources
  free_hashmap(); 
  free_pgm(&program);  
  free(rawText);
  fclose(fp);

  CCB_INFO("ENDED")
  ccb_CloseLogFile();
  return 0;
}
