#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include "scribe_c.h"

int main(int argc, char **argv) {
  char *line = NULL;
  struct timeval tv;

  if(argc < 4) {
    fprintf(stderr, "scribe_cat hostname port category\n");
    exit(1);
  }

  size_t linecap = 0;
  ssize_t linelen = 0;
  long micros = 0, prev_micros = 0;
  long line_tps = 0, prev_line_tps = 0;

  gettimeofday(&tv, NULL);
  prev_micros = micros = tv.tv_sec;

  thrift_c_t *scribe = calloc(1, sizeof(thrift_c_t));
  thrift_open(scribe, argv[1], atoi(argv[2]));

  while ( (linelen = getline(&line, &linecap, stdin)) > 0) {
    line[linelen-1] = 0;
    thrift_write(scribe, argv[3], line);
    line_tps++;
    gettimeofday(&tv, NULL);
    micros = tv.tv_sec;
    if (prev_micros != micros) {
      fprintf(stderr, "ScribeLog scribe://%s:%s/%s %ld tps\n", argv[1], argv[2], argv[3], (line_tps - prev_line_tps));
      prev_micros = micros;
      prev_line_tps = line_tps;
    }
  }
  thrift_close(scribe);
  return 0;
}
