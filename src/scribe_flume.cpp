#include <string.h>

#include "scribe_flume.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/ThriftSourceProtocol.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace flume::thrift;
using namespace flume;

int thrift_flume_open(thrift_flume_t *p, const char *host, const int port) {
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

  void *client = new ThriftSourceProtocolClient(protocol);

  transport->open();
  p->flumeClient = client;
  p->transport    = static_cast<void*>(transport.get());

  return 0;
}

int thrift_flume_write(thrift_flume_t *p, const char *category, const char *buf) {
  ThriftFlumeEvent entry;
  entry.__set_body(buf);
  int result = ((ThriftSourceProtocolClient*)p->flumeClient)->append(entry);
  return result;
}

int thrift_flume_close(thrift_flume_t *p) {
  ((TTransport*)p->transport)->close();
  delete (ThriftSourceProtocolClient*)p->flumeClient;
  memset(p, 0, sizeof(thrift_flume_t));
  return 0;
}
