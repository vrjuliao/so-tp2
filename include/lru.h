#ifndef SO_TP2_LRU
#define SO_TP2_LRU
#include <stdlib.h>
#include <stdint.h>
#include "page_table.h"

typedef struct lru_node {
  uint32_t addr;
  struct lru_node *next;
  struct lru_node *prev;
} lru_node;

void new_element(lru_node *first, lru_node *last, uint32_t addr);

void clear_lru_list(lru_node *first, size_t size);

#endif