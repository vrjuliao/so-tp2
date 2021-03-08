#include "lru.h"

void new_element(lru_node *first, lru_node *last, uint32_t addr){
  lru_node *elem = (lru_node*)malloc(sizeof(lru_node));
  elem->addr = addr;
  elem->prev = first;
  elem->next = first->next;
  first->next = elem;
}

  
// void move_elemnt(lru_node *first, lru_node *last, uint32_t addr){


// }