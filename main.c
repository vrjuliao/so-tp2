#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"


int main (int argc, const char *argv[]){
  // argparser
  features args = {argv[1], argv[2], atoi(argv[3]), atoi(argv[4])};
  
  char mode;
  uint32_t address;
  FILE *input;
  input = fopen(args.input_file, "r");

  if(!input){
    printf("ERROR: Inexistent file %s", args.input_file);
    exit(1);
  }

  int page_size = kbytes_to_bytes(args.page_size);
  int table_size = kbytes_to_bytes(args.table_size);

  // getting inputs
  while(fscanf(input, "%x %c\n", &address, &mode) != EOF){


  }
  
  fclose(input);

  /* 
  1 - create the pagetable
  2 - define which algorithm is used to access the memory
  3 - start the file reader
  4 - for each address read from the input
    4.1 - verify if this address is a page fault, a success or a dirty address.
    4.2 - compute final stats
    4.3 - execute that operation (executed by the algorithm)
  */


  printf("%s - %s - %i - %i \n", args.algorithm, args.input_file, args.page_size, args.table_size);

  return 1;
}
