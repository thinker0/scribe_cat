#include <string>
#include <vector>
#include <iostream>
#include <cctype>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <glog/logging.h>
#include "scribe_c.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/scribe.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace scribe::thrift;
using namespace scribe;

static int __thrift_write_vactor(thrift_c_t *p, const char *category, const vector<string> *lines) {
  vector<LogEntry> msgs;
  const string cate = category;
  vector<string>::const_iterator it;
  for (it = (*(lines)).begin(); it != (*(lines)).end(); ++it) {
    LogEntry *entry = new LogEntry();
    entry->category = cate;
    entry->message = *it;
    msgs.push_back(*entry);
  }

  int result = ((scribeClient*)p->scribeClient)->Log(msgs);
  msgs.clear();
  return result;
}


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
  int re = 0;
  vector<string> block_lines;

  gettimeofday(&tv, NULL);
  prev_seconds = seconds = tv.tv_sec;

  thrift_c_t *scribe = (thrift_c_t *) calloc(1, sizeof(thrift_c_t));
  // TODO (getline(cin, line))
  while ( (linelen = getline(&line, &linecap, stdin)) > 0) {
    line[linelen-1] = 0; // remove CR
    block_lines.push_back(line);
    if (block_lines.size() < 1000) {
      continue;
    }

    try {
      // int re = thrift_write(scribe, argv[3], line);
      thrift_open(scribe, argv[1], atoi(argv[2]));
      re = __thrift_write_vactor(scribe, argv[3], &block_lines);
      // TODO ResultCode::TRY_LATER == 1
      thrift_close(scribe);
      line_tps += block_lines.size();
      block_lines.clear();
    } catch (exception &e) {
      thrift_close(scribe);
      LOG(ERROR) << e.what() << "Exception..." << endl;
    } // try/catch

    gettimeofday(&tv, NULL);
    seconds = tv.tv_sec;
    if (prev_seconds != seconds) {
      fprintf(stderr, "ScribeLog scribe://%s:%s/%s %ld tps\n", argv[1], argv[2], argv[3], (line_tps - prev_line_tps));
      prev_seconds = seconds;
      prev_line_tps = line_tps;
    }
  } // while
  block_lines.clear();
  free(scribe);
  return 0;
}
