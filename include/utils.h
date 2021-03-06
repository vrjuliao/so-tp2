#ifndef SO_TP2_UTILS
#define SO_TP2_UTILS
#include <stddef.h>

#define R 'R'
#define W 'W'

typedef struct features {
  const char *algorithm;
  const char *input_file;
  int page_size;
  int table_size;
} features;



// parse the string-based hexadecimal to integer
size_t kbytes_to_bytes(int kbytes);



#endif