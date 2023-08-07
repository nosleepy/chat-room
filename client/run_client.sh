#!/bin/bash

rm client
g++ client.cpp ../common/common.cpp ../common/tcp_client.cpp -o client -pthread
./client