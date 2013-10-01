#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include "scribe_scribe.h"
#include "scribe_flume.h"

int main(int argc, char **argv) {
  char *line = NULL;
  struct timeval tv;

  if(argc < 5) {
    fprintf(stderr, "scribe_cat hostname port category protocol\n");
    exit(1);
  }

  char *protocol = argv[4];
  size_t linecap = 0;
  ssize_t linelen = 0;
  long micros = 0, prev_micros = 0;
  long line_tps = 0, prev_line_tps = 0;

  gettimeofday(&tv, NULL);
  prev_micros = micros = tv.tv_sec;

  thrift_t *protocol_type = calloc(1, sizeof(thrift_t));

  if (strstr(protocol, "scribe") == protocol) {
    thrift_scribe_open(protocol_type, argv[1], atoi(argv[2]));
  } else if (strstr(protocol, "flume") == protocol) {
    thrift_scribe_open(protocol_type, argv[1], atoi(argv[2]));
  }

  while ( (linelen = getline(&line, &linecap, stdin)) > 0) {
    line[linelen-1] = 0;
    if (strstr(protocol, "scribe") == protocol) {
      thrift_scribe_write(protocol_type, argv[3], line);
    } else if (strstr(protocol, "flume") == protocol) {
      thrift_flume_write(protocol_type, argv[3], line);
    }

    line_tps++;
    gettimeofday(&tv, NULL);
    micros = tv.tv_sec;
    if (prev_micros != micros) {
      fprintf(stderr, "ScribeLog %s://%s:%s/%s %ld tps\n", protocol, argv[1], argv[2], argv[3], (line_tps - prev_line_tps));
      prev_micros = micros;
      prev_line_tps = line_tps;
    }
  }

  if (strstr(protocol, "scribe") == protocol) {
    thrift_scribe_close(protocol_type);
  } else if (strstr(protocol, "flume") == protocol) {
    thrift_flume_close(protocol_type);
  }

  return 0;
}
