#!/bin/bash

ONCE=""
TIME=5
for param in $* ; do
  if [ "$(echo $param | grep -i once)" != "" ]; then ONCE="ONCE"; fi
  if [ $(echo $param | tr -c -d "[0-9]") -eq $param ]; then TIME=$param; fi
done

done=0
while [ $done -eq 0 ]; do
  clear
  ls -l
  echo
  echo $(cat running.txt RUNNING.TXT 2>/dev/null)
  echo "Current: $(ls -1 *.run *.RUN 2>/dev/null|tail -1|while read f;do cat $f;done)"
  if [ -e running.txt ]; then
    digits=$(cat running.txt | cut -d\  -f3 | tr -d -c '[0-9]')
    echo $(stat $digits.run | grep Change | cut -c-30 | sed "s/e/ed/")
  elif [ -e RUNNING.TXT ]; then
    digits=$(cat RUNNING.TXT | cut -d\  -f3 | tr -d -c '[0-9]' | cut -c-8)
    echo $(stat $digits.RUN | grep Birth | cut -c-30 | sed "s/Birth/Created/")
    echo $(stat $digits.RUN | grep Change | cut -c-30 | sed "s/e/ed/")
  else
    echo "Running: None"
    done=1
  fi
  echo "Storage: $(du -b pass* PASS* 2>/dev/null | awk 'BEGIN{sum=0}{sum+=$1}END{print sum}')"
  if [ "$ONCE" = "ONCE" ]; then done=1; fi
  if [ $done -eq 0 ]; then sleep $TIME; fi
done
