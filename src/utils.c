#include "utils.h"
#include <unistd.h>

size_t kbytes_to_bytes(int kbytes){
  return ((size_t)kbytes * 1024);
}

void print_stats(features *ft, stats *st){
  printf("Arquivo de entrada: %s\n", ft->input_file);
  printf("Tamanho da memoria: %i\n", ft->local_mem);
  printf("Tamanho das páginas: %i\n", ft->page_size);
  printf("Tecnica de reposicao: %s\n", ft->algorithm);
  printf("Total de acessos a memoria: %i\n", st->access);
  printf("Leituras com page faults: %i\n", st->page_faults);
  printf("Leituras sem page faults: %i\n", st->access - st->page_faults);
  printf("Paginas escritas: %i\n", st->dirty);
}

void print_table(pagetable *table, pageptr *page_pointer){
  printf("Endereco da pagina | Ultima operacao | Dirty\n");
  int addr, dirty;
  char op;
  for(int i=0; i<page_pointer->size; i++){
    addr = page_pointer->addr[i];
    op = table->page[addr].last_opetation;
    dirty = table->page[addr].dirty;
    printf("    %08X       |       %c         |  %i\n", addr, op, dirty);
  }

}