#!/bin/bash

find -maxdepth 1 -iname "running.txt" | while read f ; do
  printf "Runner: %s\n" "$(tail -1 $f)"
  echo
done

ls -1 *.run *.RUN 2>/dev/null | sort -n | while read f ; do
  echo "  File: $f"
  stat $f | grep -E 'Change|Birth' | cut -d. -f1 | sort
  printf "Status: %s\n" "$(tail -1 $f)"
  echo
done

ls -1 *.log *.LOG 2>/dev/null | sort -n | while read f ; do
  t=$(echo $f | sed -e "s/log/txt/" -e "s/LOG/TXT/")
  if [ -e "$t" ]; then
    echo $f
    0compare ~/1billion.pi $t | head -1

    bad=$(grep TmpTxt $f)
    if [ "${bad}" = "" ]; then
      grep Total $f | head -n 1
    else
      grep Result $f | head -n 1
      grep Total $f | tail -n 1 | sed "s/Total: /TmpTxt:/"
    fi

    echo
  fi
done
