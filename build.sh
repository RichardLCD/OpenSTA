#!/bin/bash

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG -DCUDD_DIR=/home/cdli/third_party/cudd-3.0.0 -DTCL_READLINE_INCLUDE=/home/cdli/third_party/tclreadline-2.3.8/include -DTCL_READLINE_LIBRARY=/home/cdli/third_party/tclreadline-2.3.8/lib/libtclreadline.so ..
make