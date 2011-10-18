#!/bin/bash

hash=$(cpp -fpreprocessed -P /dev/stdin | sed ':a;N;$!ba;s/[ \t\n]//g' | md5sum)
echo "// start: ${hash}"
cat /dev/stdin
