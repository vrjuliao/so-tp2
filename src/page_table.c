#include "page_table.h"

#include <stdio.h>
#include <unistd.h>

pagetable *create_pagetable(size_t tablesize, size_t pagesize){
  pagetable *table = (pagetable*)malloc(sizeof(pagetable));
  
  if(!table) return NULL;
  
  int num_of_pages = tablesize/pagesize;
  

  table->page = (pageattr*)malloc(num_of_pages*sizeof(pageattr));
  if (!table->page){
    free(table);
    return NULL;
  }

  uint32_t offset = 0;
  int tmp = pagesize;
  while (tmp>1) {
    tmp = tmp>>1;
    offset++;
  }

  table->offset = offset;
  table->pagesize = pagesize;
  table->tablesize = tablesize;
  table->num_of_pages = num_of_pages;

  for(int i=0; i<num_of_pages; i++){
    table->page[i].dirty = 0;
    table->page[i].is_valid = 0;
  }

  return table;
}

pageptr *create_pageptr(size_t program_memory){

  pageptr *p = (pageptr*)malloc(sizeof(pageptr));
  if(!p)
    return NULL;

  p->addr = (uint32_t*)malloc(program_memory*sizeof(pageptr));
  if (!p->addr)
    return NULL;

  p->size = program_memory;

  return p;
}

int free_pagetable(pagetable *table){
  free(table->page);
  free(table);
  return 1;
}

int free_pageptr(pageptr *page_pointer){
  free(page_pointer->addr);
  free(page_pointer);
}

uint32_t convert_address(pagetable *table, uint32_t old_address){
  return (old_address >> table->offset);
}