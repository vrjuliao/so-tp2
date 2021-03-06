#ifndef SO_TP2_PAGE_TABLE
#define SO_TP2_PAGE_TABLE
#include <stdlib.h>
#include <stdint.h>

typedef struct pageattr {
  // a complete address with 32bit is composed by the concatenation of
  // init_address and end_address;
  uint32_t end_address; // = (memory_address % pagetable.size)
  uint32_t  init_address; //  = (memory_address / pagetable.size)
  
  // flags
  uint8_t dirty;
} pageattr;

typedef struct pagetable {
  pageattr *page;
  size_t tablesize; //in bytes
  size_t pagesize; // in bytes
  size_t num_of_pages; // in bytes
} pagetable;


// allocating memory to the pagetable
// return 1 if success or else 0
// tablesize and pagesize must be given in bytes
int create_pagetable(pagetable *table, size_t tablesize, size_t pagesize);

// free memory to the pagetable
// return 1 if success or else 0
int free_pagetable(pagetable *table);

#endif