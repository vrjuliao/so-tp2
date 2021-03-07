#ifndef SO_TP2_FIFO
#define SO_TP2_FIFO
#include <stdint.h>

#include "page_table.h"

// implement an interface of accessing the fifo algorithms
// returns -1 if the page does not contain this address
int fifo_get_page_by_address(pagetable *table, uint32_t addr);


// returns 1 if a dirty page is being overwritten, or else 0
uint32_t fifo_insert_page(pagetable *table, uint32_t address);

#endif