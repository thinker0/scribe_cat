#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scribe_c.h"

int main(int argc, char **argv) {
  char buf[65536];
  
  if(argc < 4) {
    fprintf(stderr, "scribe_cat hostname port category\n");
    exit(1);
  }
  int read = 0;
  while ( (read = fread(buf, 1, 65536, stdin)) > 0) { 
    buf[read+1] = 0;
    thrift_c_t *scribe = calloc(1, sizeof(thrift_c_t));
    thrift_open(scribe, argv[1], atoi(argv[2]));
    thrift_write(scribe, argv[3], buf);
    thrift_close(scribe);
  }
  return 0;
}
