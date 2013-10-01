#ifndef __THRIFT_FLUME_H__
#define __THRIFT_FLUME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "scribe_thrift.h"

int thrift_flume_open(thrift_t *p, const char *host, const int port);
int thrift_flume_write(thrift_t *p, const char *category, const char *buf);
int thrift_flume_close(thrift_t *p);

#ifdef __cplusplus
}
#endif

#endif
