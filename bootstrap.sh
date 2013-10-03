#!/bin/sh
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

autoscan || exit 1
aclocal -I ./aclocal || exit 1
autoheader || exit 1

if libtoolize --version 1 >/dev/null 2>/dev/null; then
  libtoolize --copy --automake || exit 1
elif glibtoolize --version 1 >/dev/null 2>/dev/null; then
  glibtoolize --copy --automake || exit 1
fi

autoreconf --force --verbose --install
automake -ac --include-deps --add-missing --foreign || exit 1

# To be safe include -I flag
./configure --config-cache $*

thrift -o src/ -I ./if --gen cpp:pure_enums --gen c_glib ./if/fb303.thrift
thrift -o src/ -I ./if --gen cpp:pure_enums --gen c_glib ./if/scribe.thrift
thrift -o src/ -I ./if --gen cpp:pure_enums --gen c_glib ./if/flume.thrift
thrift -o src/ -I ./if --gen cpp:pure_enums --gen c_glib --gen java ./if/bucketupdater.thrift
