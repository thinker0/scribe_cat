
all:

bin_PROGRAMS = scribe_cat
scribe_cat_SOURCES = scribe_c.cpp scribe_cat.cpp
scribe_cat_SOURCES += gen-cpp/FacebookService.cpp gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp 
scribe_cat_SOURCES += gen-cpp/scribe.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe_types.cpp
scribe_cat_SOURCES += gen-cpp/flume_constants.cpp gen-cpp/flume_types.cpp gen-cpp/ThriftSourceProtocol.cpp
scribe_cat_CFLAGS = $(THRIFT_CFLAGS) $(THRIFT_NB_CFLAGS) $(THRIFT_Z_CFLAGS) $(AM_CFLAGS)
scribe_cat_CFLAGS += $(GLOG_CFLAGS)
scribe_cat_LDFLAGS = $(THRIFT_LIBS) $(THRIFT_NB_LIBS) $(THRIFT_Z_LIBS) $(AM_LDFLAGS)
scribe_cat_LDFLAGS += $(GLOG_LIBS)

thrift -o . -I ../if --gen cpp:pure_enums --gen c_glib ../if/fb303.thrift
thrift -o . -I ../if --gen cpp:pure_enums --gen c_glib ../if/scribe.thrift
