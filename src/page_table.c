#include "page_table.h"

int create_pagetable(pagetable *table, size_t tablesize, size_t pagesize){
  table = (pagetable*)malloc(sizeof(pagetable));
  if(!table) return 0;
  
  int num_of_pages = tablesize/pagesize;

  table->page = malloc(num_of_pages*sizeof(pageattr));
  if (!table->pagesize){
    free(table);
    return 0;
  }

  table->pagesize = pagesize;
  table->tablesize = tablesize;
  table->num_of_pages = num_of_pages;

  for(int i=0; i<num_of_pages; i++){
    table->page[i].init_address = i;
    table->page[i].dirty = 0;
  }

  return 1;
}

int free_pagetable(pagetable *table){
  free(table->page);
  free(table);
  return 1;
}