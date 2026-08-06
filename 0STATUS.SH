#!/bin/bash

NOCHECK=""
for param in $* ; do
  TEST=$(echo "$param" | grep -i NOCHECK)
  if [ "$TEST" != "" ]; then NOCHECK="NOCHECK"; fi
done

ls -1 *.log *.LOG 2>/dev/null | sort -n | while read log ; do
  bad=$(grep TmpTxt $log)
  run=$(echo $log | sed -e "s/log/run/" -e "s/LOG/RUN/")
  txt=$(echo $log | sed -e "s/log/txt/" -e "s/LOG/TXT/")

  echo $log

  if [ "${bad}" = "" ]; then
    grep -E 'Result|Write|Total' $log
  else
    grep -E 'Result|Write' $log
    grep -E 'Total' $log | tail -n 1 | sed "s/Total: /TmpTxt:/"
  fi

  if [ -e $run ]; then
    stat $run | grep -E 'Change|Birth' | sort | cut -c-30
    cat $run
    echo
  else
    stat $log | grep -E 'Change|Birth' | sort | cut -c-30
  fi

  if [ -e $txt -a "$NOCHECK" != "NOCHECK" ]; then
    0compare ~/1billion.pi $txt good
  fi

  echo
done

find -maxdepth 1 -iname "running.txt" | while read f ; do
  tail -1 $f
  echo
done

