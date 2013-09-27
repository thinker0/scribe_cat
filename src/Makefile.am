
all:

thrift -o . -I ../if --gen cpp:pure_enums ../if/fb303.thrift
thrift -o . -I ../if --gen cpp:pure_enums ../if/scribe.thrift

AM_CFLAGS = $(pkg-config --cflags --libs thrift)
AM_LDFLAGS = $(pkg-config --cflags --libs thrift)

bin_PROGRAMS = scribe_cat
scribe_cat_SOURCES = scribe_c.cpp scribe_cat.c
scribe_cat_SOURCES += gen-cpp/FacebookService.cpp gen-cpp/fb303_constants.cpp gen-cpp/fb303_types.cpp 
scribe_cat_SOURCES += gen-cpp/scribe.cpp gen-cpp/scribe_constants.cpp gen-cpp/scribe_types.cpp
scribe_cat_CFLAGS = $(AM_CFLAGS)
scribe_cat_LDFLAGS = $(AM_LDFLAGS)