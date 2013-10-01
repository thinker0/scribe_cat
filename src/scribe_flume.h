#ifndef __THRIFT_FLUME_H__
#define __THRIFT_FLUME_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct thrift_flume_t {
  char *host;
  int   port;

  //  ThriftSourceProtocolClient *flumeClient;
  void *flumeClient;

  //TTransport   *transport;
  void *transport;
} thrift_flume_t, *thrift_flume_t_ptr;

int thrift_flume_open(thrift_flume_t *p, const char *host, const int port);
int thrift_flume_write(thrift_flume_t *p, const char *category, const char *buf);
int thrift_flume_close(thrift_flume_t *p);

#ifdef __cplusplus
}
#endif

#endif
