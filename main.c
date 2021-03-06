#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"
#include "page_table.h"

#define alg_fifo "fifo"
#define alg_2a "2a"
#define alg_lru "lru"
#define alg_custom "custom"



void execute_fifo(FILE *input, pagetable *table);
void execute_lru(FILE *input, pagetable *table);
void execute_2a(FILE *input, pagetable *table);
void execute_custom(FILE *input, pagetable *table);

/* 
  1 - start the file reader
  2 - create the pagetable
  3 - define which algorithm is used to access the memory
  4 - for each address read from the input
    4.1 - verify if this address is a page fault, a success or a dirty address.
    4.2 - compute final stats
    4.3 - execute that operation (executed by the algorithm)
  5 - print stats
*/
int main (int argc, const char *argv[]){
  // argparser
  features args = {argv[1], argv[2], atoi(argv[3]), atoi(argv[4])};
  
  // 1. starting file reader
  FILE *input;
  input = fopen(args.input_file, "r");

  if(!input){
    printf("ERROR: Inexistent file %s", args.input_file);
    exit(1);
  }

  // 2. create the page table
  size_t page_size = kbytes_to_bytes(args.page_size);
  size_t table_size = kbytes_to_bytes(args.table_size);

  pagetable *table;
  if (!create_pagetable(table, table_size, page_size)){
    printf("ERROR: memory allocation problem");
    exit(1);
  }
  
  // 3. choose the algorithm
  void(* aceess_memory)(pagetable*);
    
  if(!strcmp(args.algorithm, alg_fifo))
    execute_fifo(input, table);
  else if(!strcmp(args.algorithm, alg_lru))
    execute_lru(input, table);
  else if(!strcmp(args.algorithm, alg_2a))
    execute_2a(input, table);
  else if(!strcmp(args.algorithm, alg_custom))
    execute_custom(input, table);
  else {
    printf("ERROR: Inexistent algorithm %s", args.algorithm);
    exit(1);
  }

  fclose(input);


  free_pagetable(table);
  // 5. TODO: print stats 
  printf("%s - %s - %i - %i \n", args.algorithm, args.input_file, args.page_size, args.table_size);

  return 1;
}

void execute_fifo(FILE *input, pagetable *table){
  char mode;
  uint32_t address;
  while(fscanf(input, "%x %c\n", &address, &mode) != EOF){


  }
  
}

void execute_lru(FILE *input, pagetable *table){}
void execute_2a(FILE *input, pagetable *table){}
void execute_custom(FILE *input, pagetable *table){}