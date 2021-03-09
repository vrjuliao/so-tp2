#define  _POSIX_C_SOURCE 200809L
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

size_t kbytes_to_bytes(int kbytes){
  return ((size_t)kbytes * 1024);
}

void print_stats(features *ft, stats *st){
  printf("Arquivo de entrada: %s\n", ft->input_file);
  printf("Tamanho da memoria: %i KB\n", ft->local_mem);
  printf("Tamanho das paginas: %i KB\n", ft->page_size);
  printf("Tecnica de reposicao: %s\n", ft->algorithm);
  printf("Total de acessos a memoria: %i\n", st->access);
  printf("Leituras com page faults: %i\n", st->page_faults);
  printf("Leituras sem page faults: %i\n", st->access - st->page_faults);
  printf("Paginas (sujas) escritas: %i\n", st->dirty);
}

int read_input_from_file(FILE *input, uint32_t *addr, char *mode){
  char buff[15];
  char *line = buff;
  size_t buffer_size = 15;
  
  int line_size;
  while(EOF != (line_size = getline(&line, &buffer_size, input))){
    sscanf(buff, "%x %c", addr, mode);
    if((*mode) == R || (*mode) == W){
      return line_size;
    }
  }
  return EOF;
}

void print_table(pagetable *table, pageptr *page_pointer){
  printf("Endereco da pagina | Ultima operacao | Dirty\n");
  int addr, dirty;
  char op;
  for(int i=0; i<page_pointer->size; i++){
    addr = page_pointer->addr[i];
    op = table->page[addr].last_opetation;
    dirty = table->page[addr].dirty;
    printf("    0x%08x     |       %c         |  %i\n", addr<<table->offset, op, dirty);
  }

}