#!/bin/bash

input=$(cat /dev/stdin)
hash=$(cpp -fpreprocessed -P <(echo "$input") | sed ':a;N;$!ba;s/[ \t\n]//g' | md5sum)
echo "// start: ${hash}"
echo "$input"
echo "// end"
