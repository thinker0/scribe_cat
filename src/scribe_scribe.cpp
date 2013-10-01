#include <string.h>

#include "scribe_scribe.h"

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

int thrift_scribe_open(thrift_scribe_t *p, const char *host, const int port) {
  p->host = strdup(host);
  p->port = port;

  boost::shared_ptr<TSocket> socket(new TSocket(p->host, p->port));
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
  p->transport    = static_cast<void*>(transport.get());

  return 0;
}

int thrift_scribe_write(thrift_scribe_t *p, const char *category, const char *buf) {
  LogEntry entry;
  entry.category = category;
  entry.message = buf;

  std::vector<LogEntry> msgs;
  msgs.push_back(entry);

  int result = ((scribeClient*)p->scribeClient)->Log(msgs);
  return result;
}

int thrift_scribe_close(thrift_scribe_t *p) {
  ((TTransport*)p->transport)->close();
  delete (scribeClient*)p->scribeClient;
  memset(p, 0, sizeof(thrift_scribe_t));
  return 0;
}

