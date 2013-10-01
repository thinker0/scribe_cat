#ifndef __THRIFT_C_H__
#define __THRIFT_C_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct thrift_scribe_t {
  char *host;
  int   port;

  //  ScribeClient *scribeClient;
  void *scribeClient;

  //TTransport   *transport;
  void *transport;
} thrift_scribe_t, *thrift_scribe_t_ptr;

int thrift_scribe_open(thrift_scribe_t *p, const char *host, const int port);
int thrift_scribe_write(thrift_scribe_t *p, const char *category, const char *buf);
int thrift_scribe_close(thrift_scribe_t *p);

#ifdef __cplusplus
}
#endif

#endif
