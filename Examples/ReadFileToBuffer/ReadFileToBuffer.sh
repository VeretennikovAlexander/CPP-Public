#/bin/bash

rm ./ReadFileToBuffer

clang++ -std=c++23 ReadFileToBuffer.cpp -o ReadFileToBuffer

chmod +x -- ReadFileToBuffer
./ReadFileToBuffer