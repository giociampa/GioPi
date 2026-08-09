#!/bin/bash

done=0
while [ $done -eq 0 ]; do
  clear
  ls -l
  echo
  echo $(cat running.txt 2>/dev/null)
  echo "Current: $(ls -1 *.run|tail -1|while read f;do cat $f;done)"
  if [ -e running.txt ]; then
    digits=$(cat running.txt | cut -d\  -f3)
    echo $(stat $digits.run | grep Change | cut -c-30 | sed "s/e/ed/")
  else
    echo "Running: None"
    done=1
  fi
  echo "Storage: $(du -b pass* 2>/dev/null | awk 'BEGIN{sum=0}{sum+=$1}END{print sum}')"
  if [ $done -eq 0 ]; then sleep 5; fi
done
