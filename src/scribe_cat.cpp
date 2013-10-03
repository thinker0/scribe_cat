#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <glog/logging.h>
#include "scribe_c.h"

using namespace std;

int main(int argc, char **argv) {
  char *line = NULL;
  struct timeval tv;

  if(argc < 4) {
    fprintf(stderr, "scribe_cat hostname port category\n");
    exit(1);
  }
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  size_t linecap = 0;
  ssize_t linelen = 0;
  long seconds = 0, prev_seconds = 0;
  long line_tps = 0, prev_line_tps = 0;

  gettimeofday(&tv, NULL);
  prev_seconds = seconds = tv.tv_sec;

  thrift_c_t *scribe = (thrift_c_t *) calloc(1, sizeof(thrift_c_t));
  thrift_open(scribe, argv[1], atoi(argv[2]));

  while ( (linelen = getline(&line, &linecap, stdin)) > 0) {
    line[linelen-1] = 0; // remove LF
    try {
      thrift_write(scribe, argv[3], line);
    } catch (...) {
      thrift_close(scribe);
      thrift_open(scribe, argv[1], atoi(argv[2]));
      LOG(ERROR) << "Exception..." << endl;
    }

    line_tps++;
    gettimeofday(&tv, NULL);
    seconds = tv.tv_sec;
    if (prev_seconds != seconds) {
      fprintf(stderr, "ScribeLog scribe://%s:%s/%s %ld tps\n", argv[1], argv[2], argv[3], (line_tps - prev_line_tps));
      prev_seconds = seconds;
      prev_line_tps = line_tps;
    }
  }
  thrift_close(scribe);
  free(scribe);
  return 0;
}
