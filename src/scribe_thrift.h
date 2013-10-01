#ifndef __SCRIBE_THRIFT_H__
#define __SCRIBE_THRIFT_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct thrift_t {
  char *host;
  int   port;

  //  ThriftSourceProtocolClient *flumeClient;
  void *client;

  //TTransport   *transport;
  void *transport;
} thrift_t, *thrift_t_ptr;

#ifdef __cplusplus
}
#endif

#endif
