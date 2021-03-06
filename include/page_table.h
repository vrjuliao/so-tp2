#ifndef SO_TP2_UTILS
#define SO_TP2_UTILS
#include <stdlib.h>
#include <stdint.h>

typedef struct pageattr {
  // a complete address with 32bit is composed by the concatenation of
  // init_address and end_address;
  uint32_t end_address; // = (memory_address % pagetable.size)
  uint32_t  init_address; //  = (memory_address / pagetable.size)
  
  // put the attributes of an element of the page
  // control bits
} pageattr;

typedef struct pagetable {
  pageattr *page;
  size_t tablesize; //in bytes
  size_t pagesize; // in bytes
} pagetable;


// allocating memory to the pagetable
// return 1 if success or else 0
// tablesize and pagesize must be given in bytes
int create_pagetable(pagetable *pg, size_t tablesize, size_t pagesize);

// free memory to the pagetable
// return 1 if success or else 0
int free_pagetable(pagetable *pg, size_t size);


// calculating the address in the pagetable and checking if such address is the corre
// int get_address(pagetable *pg, size_t size, uint32_t address);


#endif