#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

#include "utils.h"
#include "page_table.h"
#include "fifo.h"
#include "lru.h"

#define alg_fifo "fifo"
#define alg_2a "2a"
#define alg_lru "lru"
#define alg_custom "custom"

stats execute_fifo(FILE *input, pagetable *table, pageptr *page_pointer);
stats execute_lru(FILE *input, pagetable *table, pageptr *page_pointer);
stats execute_2a(FILE *input, pagetable *table, pageptr *page_pointer);
stats execute_custom(FILE *input, pagetable *table, pageptr *page_pointer);

int help(const char program[]){
  printf("usage: %s [algoritmo] file.log page_size mem_size\n", program);
  return 0;
}

int main (int argc, const char *argv[]){
  if (argc < 5)
    return help(argv[0]);

  // argparser
  features args = {argv[1], argv[2], atoi(argv[3]), atoi(argv[4])};
  
  // 1. starting file reader
  FILE *input;
  input = fopen(args.input_file, "r");
  if(!input){
    printf("ERROR: Inexistent file %s\n", args.input_file);
    exit(1);
  }

  // 2. create the page table
  size_t page_size = kbytes_to_bytes(args.page_size);
  size_t table_size = ((uint32_t)-1);
  size_t program_memroy_size = kbytes_to_bytes(args.local_mem)/page_size;
  
  pagetable *table = create_pagetable(table_size, page_size);
  if (!table){
    printf("ERROR: memory allocation problem\n");
    exit(1);
  }
  pageptr *page_pointer = create_pageptr(program_memroy_size);
  if (!page_pointer){
    printf("ERROR: memory allocation problem\n");
    free_pagetable(table);
    exit(1);
  }
  // start timer
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);

  // 3. choose the algorithm
  stats execution_stats; 
  if(!strcmp(args.algorithm, alg_fifo)){
    execution_stats = execute_fifo(input, table, page_pointer);
  } else if(!strcmp(args.algorithm, alg_lru))
    execution_stats = execute_lru(input, table, page_pointer);
  else if(!strcmp(args.algorithm, alg_2a))
    execution_stats = execute_2a(input, table, page_pointer);
  else if(!strcmp(args.algorithm, alg_custom))
    execution_stats = execute_custom(input, table, page_pointer);
  else {
    printf("ERROR: Inexistent algorithm %s", args.algorithm);
    free_pagetable(table);
    free_pageptr(page_pointer);
    exit(1);
  }
  gettimeofday(&t2, NULL);  
  fclose(input);

  // 5. TODO: print stats 
  double elapsedTime;
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
  print_stats(&args, &execution_stats);
  printf("Tempo de execucao em segundos: %lf\n", elapsedTime/1000.0);
  print_table(table, page_pointer);

  free_pagetable(table);
  free_pageptr(page_pointer);
  return 0;
}

stats execute_fifo(FILE *input, pagetable *table, pageptr *page_pointer){
  char mode;
  stats st = {0, 0, 0};
  uint32_t newaddress, oldaddress;
  size_t mem_size = page_pointer->size;

  unsigned int last_added = 0;
  pageattr *p;
  while(EOF != fscanf(input, "%x %c\n", &newaddress, &mode)){
    st.access++;
    newaddress = convert_address(table, newaddress);

    if(!valid_page(table, newaddress)){
      st.page_faults++;
      if(last_added >= mem_size){

        oldaddress = page_pointer->addr[last_added%mem_size];
        if(is_dirty(table, oldaddress)){
          st.dirty++;
        }
        remove_page(table, oldaddress);
      }
      new_page(table, newaddress, 1);
      page_pointer->addr[last_added%mem_size] = newaddress;
      last_added++;
    }
    p = &table->page[newaddress];

    if(mode == W){
      p->dirty = 1;
    }
    p->last_opetation = mode;
  }
  return st;
}

stats execute_lru(FILE *input, pagetable *table, pageptr *page_pointer){
  char mode;
  stats st = {0, 0, 0};
  uint32_t newaddress, last, first, tmp;
  size_t mem_size = page_pointer->size;
  
  if(EOF != fscanf(input, "%x %c\n", &newaddress, &mode)){
    st.access++;
    first = convert_address(table, newaddress);
    last = first;
    new_page(table, first, -1);
  }

  unsigned int qtt = 0;
  pageattr *p;
  while(EOF != fscanf(input, "%x %c\n", &newaddress, &mode)){
    st.access++;
    newaddress = convert_address(table, newaddress);

    if(!valid_page(table, newaddress)){
      st.page_faults++;
      if(qtt >= mem_size){
        if(is_dirty(table, last)){
          st.dirty++;
        }
        tmp = table->page[last].prev;
        remove_page(table, last);
        last = tmp;
      }
      new_page(table, newaddress, first);
      first = newaddress;
      qtt++;
    } else if (newaddress != first){
      
      switch_prev_next(table, newaddress);
      table->page[first].prev = newaddress;
      table->page[newaddress].next = first;
      
      first = newaddress;
      if(newaddress == last && table->page[newaddress].prev != -1)
        last = table->page[newaddress].prev;
    }
    p = &table->page[newaddress];

    if(mode == W){
      p->dirty = 1;
    }
    p->last_opetation = mode;
  }

  uint32_t current = first;
  for(int i=0; i<mem_size; i++){
    page_pointer->addr[i] = current;
    current = table->page[current].next;
  }

  return st;
}


stats execute_2a(FILE *input, pagetable *table, pageptr *page_pointer){}
stats execute_custom(FILE *input, pagetable *table, pageptr *page_pointer){}