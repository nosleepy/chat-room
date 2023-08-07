#!/bin/bash

rm server
g++ server.cpp ../common/common.cpp ../common/tcp_server.cpp -o server -pthread
./server