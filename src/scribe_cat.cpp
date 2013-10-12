#include <string>
#include <vector>
#include <iostream>
#include <cctype>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

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

typedef struct thrift_c_t {
  const char *host;
  int   port;
  scribeClient *scribeClient;
  TTransport   *transport;
} thrift_c_t, *thrift_c_t_ptr;


int __thrift_open(thrift_c_t *p, const string *host, const int port) {
  p->host = host->c_str();
  p->port = port;

  const boost::shared_ptr<TSocket> socket(new TSocket(p->host, p->port));
  socket->setLinger(true,1);
  socket->setNoDelay(true);
  socket->setConnTimeout(100);
  socket->setSendTimeout(100);
  socket->setRecvTimeout(100);
  socket->setMaxRecvRetries(3);
  boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
  boost::shared_ptr<TProtocol>  protocol(new TBinaryProtocol(transport));

  scribeClient *client = new scribeClient(protocol);

  transport->open();
  p->scribeClient = client;
  p->transport    = transport.get();

  return 0;
}

static int __thrift_write_vactor(thrift_c_t *p, const string *category, const vector<string> *lines) {
  vector<LogEntry> msgs;
  vector<string>::const_iterator it;
  for (it = (*(lines)).begin(); it != (*(lines)).end(); ++it) {
    LogEntry *entry = new LogEntry();
    entry->category = *category;
    entry->message = *it;
    msgs.push_back(*entry);
  }

  int result = p->scribeClient->Log(msgs);
  msgs.clear();
  return result;
}

int __thrift_close(thrift_c_t *p) {
  p->transport->close();
  delete p->scribeClient;
  memset(p, 0, sizeof(thrift_c_t));
  return 0;
}

void print_usage(const char* program_name) {
  cout << "Usage: " << program_name << " [-h host] [-p port] [-c config_file]" << endl;
}

int main(int argc, char **argv) {

  if(argc < 4) {
    print_usage(argv[0]);
    exit(1);
  }

  int next_option;
  const char* const short_options = "hpc:";
  const struct option long_options[] = {
    { "host",     1, NULL, 'h' },
    { "port",     1, NULL, 'p' },
    { "category", 1, NULL, 'c' },
    { NULL,       0, NULL, 0   },
  };

  unsigned long int port = 1463;
  string category;
  string host = "localhost";

  while (0 < (next_option = getopt_long(argc, argv, short_options, long_options, NULL))) {
    switch (next_option) {
    case 'h':
      host = optarg;
      break;
    case 'c':
      category = optarg;
      break;
    case 'p':
      port = strtoul(optarg, NULL, 0);
      break;
    }
  }

  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  char *line = NULL;
  struct timeval tv;
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
      __thrift_open(scribe, &host, port);
      re = __thrift_write_vactor(scribe, &category, &block_lines);
      // TODO ResultCode::TRY_LATER == 1
      __thrift_close(scribe);
      line_tps += block_lines.size();
      block_lines.clear();
    } catch (exception &e) {
      __thrift_close(scribe);
      LOG(ERROR) << e.what() << "Exception..." << endl;
    } // try/catch

    gettimeofday(&tv, NULL);
    seconds = tv.tv_sec;
    if (prev_seconds != seconds) {
      fprintf(stderr, "ScribeLog scribe://%s:%lu/%s %ld tps\n", host.c_str(), port, category.c_str(), (line_tps - prev_line_tps));
      prev_seconds = seconds;
      prev_line_tps = line_tps;
    }
  } // while
  block_lines.clear();
  free(scribe);
  return 0;
}
