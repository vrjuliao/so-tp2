#ifndef SO_TP2_UTILS
#define SO_TP2_UTILS
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "page_table.h"

#define R 'R'
#define W 'W'

typedef struct features {
  const char *algorithm;
  const char *input_file;
  int page_size;
  int local_mem;
} features;

typedef struct stats {
  int page_faults;
  int dirty;
  int access;
} stats;

// parse the string-based hexadecimal to integer
size_t kbytes_to_bytes(int kbytes);

void print_stats(features *ft, stats *st);
void print_table(pagetable *table, pageptr *page_pointer);

#endif