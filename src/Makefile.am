
all:

bin_PROGRAMS = scribe_cat
scribe_cat_SOURCES = scribe_cat.cpp
scribe_cat_SOURCES += gen-cpp/FacebookService.cpp gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp 
scribe_cat_SOURCES += gen-cpp/scribe.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe_types.cpp
scribe_cat_SOURCES += gen-cpp/BucketStoreMapping.cpp gen-cpp/bucketupdater_constants.cpp gen-cpp/bucketupdater_types.cpp
scribe_cat_SOURCES += gen-cpp/flume_constants.cpp gen-cpp/flume_types.cpp gen-cpp/ThriftSourceProtocol.cpp
scribe_cat_CFLAGS = $(THRIFT_CFLAGS) $(THRIFT_NB_CFLAGS) $(AM_CFLAGS)
scribe_cat_CFLAGS += $(GLOG_CFLAGS)
scribe_cat_LDFLAGS = $(THRIFT_LIBS) $(THRIFT_NB_LIBS) $(AM_LDFLAGS)
scribe_cat_LDFLAGS += $(GLOG_LIBS)

thrift -o . -I ../if --gen cpp:pure_enums --gen c_glib --gen java ../if/fb303.thrift
thrift -o . -I ../if --gen cpp:pure_enums --gen c_glib --gen java ../if/scribe.thrift
thrift -o . -I ../if --gen cpp:pure_enums --gen c_glib --gen java ../if/flume.thrift
thrift -o . -I ../if --gen cpp:pure_enums --gen c_glib --gen java ../if/bucketupdater.thrift

