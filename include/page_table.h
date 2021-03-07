#ifndef SO_TP2_PAGE_TABLE
#define SO_TP2_PAGE_TABLE
#include <stdlib.h>
#include <stdint.h>

typedef struct pageattr {
  // flags
  uint8_t dirty;
  uint8_t is_valid;
  char last_opetation;
} pageattr;

typedef struct pageptr{
  uint32_t* addr;
  size_t size;
} pageptr;

typedef struct pagetable {
  pageattr *page;
  size_t tablesize; //in bytes
  size_t pagesize; // in bytes
  size_t num_of_pages; // in bytes
  uint32_t offset;
} pagetable;


// allocating memory to the pagetable
// return 1 if success or else 0
// tablesize and pagesize must be given in bytes
pagetable *create_pagetable(size_t tablesize, size_t pagesize);
pageptr *create_pageptr(size_t program_memory);
// free memory to the pagetable
// return 1 if success or else 0
int free_pagetable(pagetable *table);
int free_pageptr(pageptr *page_pointer);

uint32_t convert_address(pagetable *table, uint32_t old_address);

#endif