#! /bin/bash

cmake --preset release
cmake --build --preset release
ctest --preset release --output-junit /tmp/test-results.xml
cat /tmp/test-results.xml