#!/bin/bash

while true ; do
  clear
  ls -l
  echo
  echo $(cat running.txt 2>/dev/null)
  echo "Current: $(cat *.run|tail -1)"
  echo "Storage: $(du -b pass* 2>/dev/null | awk 'BEGIN{sum=0}{sum+=$1}END{print sum}')"
  sleep 5
done
