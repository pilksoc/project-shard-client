#!/bin/bash

find ./src ./tests | grep -E ".*\\.[ch](pp)?\$" | xargs astyle --style=kr
cmake-format ./CMakeLists.txt > ./CMakeLists.txt.orig && cp ./CMakeLists.txt.orig ./CMakeLists.txt
