#ifndef SO_TP2_PAGE_TABLE
#define SO_TP2_PAGE_TABLE
#include <stdlib.h>
#include <stdint.h>

typedef struct pageattr {
  // flags
  uint8_t dirty;
  uint8_t is_valid;
  char last_opetation;

  //used by LRU method
  int64_t next, prev;
} pageattr;

typedef struct pageptr{
  uint32_t* addr;
  size_t size;
} pageptr;

typedef struct pagetable {
  pageattr *page;
  size_t tablesize; //in bytes
  size_t pagesize; // in bytes
  size_t num_of_pages;
  uint32_t offset;
} pagetable;


// memory allocation
pagetable *create_pagetable(size_t tablesize, size_t pagesize);
pageptr *create_pageptr(size_t program_memory);

// free memory
int free_pagetable(pagetable *table);
int free_pageptr(pageptr *page_pointer);

// address offset shifiting
uint32_t convert_address(pagetable *table, uint32_t old_address);

// checking if an address is active on tablepage
int valid_page(pagetable* table, uint32_t address);

// checking if some page was written or not
int is_dirty(pagetable* table, uint32_t address);

// removing a page from the tablepage
void remove_page(pagetable *table, uint32_t address);

// adding a page
void new_page(pagetable *table, uint32_t address, int64_t next_addr);

/* 
Used for the LRU method, replace the pointers when an page
is being moved from the table.
*/
void switch_prev_next(pagetable *table, uint32_t address);
#endif