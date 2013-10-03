#ifndef __THRIFT_C_H__
#define __THRIFT_C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct thrift_c_t {
  char *host;
  int   port;

  //  ScribeClient *scribeClient;
  void *scribeClient;

  //TTransport   *transport;
  void *transport;
} thrift_c_t, *thrift_c_t_ptr;

int thrift_open(thrift_c_t *p, const char *host, const int port);
int thrift_write(thrift_c_t *p, const char *category, const char *buf);
int thrift_close(thrift_c_t *p);

#ifdef __cplusplus
}
#endif

#endif
