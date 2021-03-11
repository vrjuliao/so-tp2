#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

#include "utils.h"
#include "page_table.h"

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

  // 2. creating page table
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

  // starting timer
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);

  // 3. choosing algorithm
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

  // 4. printing stats 
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

  // identifies which element should be removed from the fifo sequence
  unsigned int last_added = 0;

  pageattr *p;
  while(EOF != read_input_from_file(input, &newaddress, &mode)){
    st.access++;
    //applying the offest shifting
    newaddress = convert_address(table, newaddress);

    if(!valid_page(table, newaddress)){
      st.page_faults++;
      
      // it means that he fifo list is full, then we need remove an element
      if(last_added >= mem_size){

        // getting address to remove
        oldaddress = page_pointer->addr[last_added%mem_size];
        if(is_dirty(table, oldaddress)){
          st.dirty++;
        }
        remove_page(table, oldaddress);
      }
      new_page(table, newaddress, 1);
      //adding a new page on fifo sequence
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
  
  /*
  since it is necessary a first element on the LRU reference pages
  we read the first access from the file separetelly from the others.
  */ 
  // if(EOF != fscanf(input, "%x %c\n", &newaddress, &mode)){
  if(EOF != read_input_from_file(input, &newaddress, &mode)){
    st.access++;
    first = convert_address(table, newaddress);
    last = first;
    new_page(table, first, -1);
  }

  // quantity of added elements on tablepage.
  unsigned int qtt = 1;
  pageattr *p;
  while(EOF != read_input_from_file(input, &newaddress, &mode)){  
    st.access++;

    //applying the offest shifting
    newaddress = convert_address(table, newaddress);

    if(!valid_page(table, newaddress)){
      st.page_faults++;
      
      // it means that he fifo list is full, then we need remove an element
      if(qtt >= mem_size){
        if(is_dirty(table, last)){
          st.dirty++;
        }

        // removing the Last Recentely used page and replacing the pointers
        tmp = table->page[last].prev;
        remove_page(table, last);
        last = tmp;
      }

      //insertihn the added page at the top of the LRU sequence
      new_page(table, newaddress, first);
      first = newaddress;
      qtt++;
    } else if (newaddress != first){
      /*
      if a page already in the pagetable is accessed, it will me moved to
      the top of LRU sequence
      */
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

  // place the used pages on the page_pointe to print such list
  uint32_t current = first;
  for(int i=0; i<mem_size; i++){
    page_pointer->addr[i] = current;
    current = table->page[current].next;
  }

  return st;
}

stats execute_2a(FILE *input, pagetable *table, pageptr *page_pointer){
  char mode;
  stats st = {0, 0, 0};
  uint32_t newaddress, oldaddress;
  size_t mem_size = page_pointer->size;

  // identifies which element should be removed from the 2nd-chance sequence
  uint64_t last_added = 0;
  pageattr *p;
  // while(EOF != fscanf(input, "%x %c\n", &newaddress, &mode)){
  while(EOF != read_input_from_file(input, &newaddress, &mode)){
    st.access++;
    
    //applying the offest shifting
    newaddress = convert_address(table, newaddress);

    if(!valid_page(table, newaddress)){
      st.page_faults++;
      // it means that he 2nd-chance list is full, then we need remove an element
      if(last_added >= mem_size){

        /*
        getting address to remove
        Here, the `is_valid` integer is used also to validate the chance of some
        page.
        For `page->is_valid` = 0, the page is not in the tablepage.
        If some page has is_valid=2, the this one has 2 chances, and is_valid=1
        means that this page has 1 (the last) chance.
        */
        p = &table->page[page_pointer->addr[last_added%mem_size]];
        while(p->is_valid > 1){
          last_added++;
          p->is_valid--;
          p = &table->page[page_pointer->addr[last_added%mem_size]];
        }

        oldaddress = page_pointer->addr[last_added%mem_size];
        if(is_dirty(table, oldaddress)){
          st.dirty++;
        }
        remove_page(table, oldaddress);
      }
      //adding a new page on 2nd-chance sequence
      new_page(table, newaddress, 1);
      page_pointer->addr[last_added%mem_size] = newaddress;
      last_added++;
    }
    p = &table->page[newaddress];

    if(mode == W){
      p->dirty = 1;
    }
    // when a page is accessed, the number of chances is restarted
    table->page[newaddress].is_valid = 2;
    p->last_opetation = mode;
  }
  return st;
}

stats execute_custom(FILE *input, pagetable *table, pageptr *page_pointer){
  char mode;
  stats st = {0, 0, 0};
  uint32_t newaddress, oldaddress, virtual_address;
  size_t mem_size = page_pointer->size;

  pageattr *p;
  while(EOF != read_input_from_file(input, &newaddress, &mode)){
    st.access++;
     
    //applying the offest shifting
    newaddress = convert_address(table, newaddress);
    if(!valid_page(table, newaddress)){
      st.page_faults++;
      /* 
      The space available on the tablepage does not matters for this approach.
      The virtual address is given by the least significant bits of the
      physical addres, then if two physical address have the same virtual
      address, then they will be replace each other on the table page.
      */
      // it works only if mem_size is a number in pow2 sequence.
      virtual_address = newaddress & (mem_size-1);
      oldaddress = page_pointer->addr[virtual_address];
      if(is_dirty(table, oldaddress)){
        st.dirty++;
      }
      remove_page(table, oldaddress);
      new_page(table, newaddress, 1);
      page_pointer->addr[virtual_address] = newaddress;
    }
    p = &table->page[newaddress];

    if(mode == W){
      p->dirty = 1;
    }
    p->last_opetation = mode;
  }
  return st;
}