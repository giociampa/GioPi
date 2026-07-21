#!/bin/bash

find -maxdepth 1 -iname "running.txt" | while read f ; do
  printf "Runner: %s\n" "$(tail -1 $f)"
  echo
done

ls -1 *.run *.RUN 2>/dev/null | sort -n | while read f ; do
  echo "  File: $f"
  stat $f | grep -E 'Change|Birth' | sort | cut -c-30
  printf "Status: %s\n" "$(tail -1 $f)"
  echo
done

ls -1 *.log *.LOG 2>/dev/null | sort -n | while read f ; do
  echo $f

  r=$(echo $f | sed -e "s/log/raw/" -e "s/LOG/RAW/")
  t=$(echo $f | sed -e "s/log/txt/" -e "s/LOG/TXT/")
  if [ -e "$r" -a ! -e "$t" ]; then 0raw2txt $r ; fi

  if [ -e "$t" ]; then
    0compare ~/1billion.pi $t | head -n 1

    bad=$(grep TmpTxt $f)
    grep Result $f | head -n 1
    if [ "${bad}" = "" ]; then
      grep -E "Write|Total" $f
    else
      grep Total $f | tail -n 1 | sed "s/Total: /TmpTxt:/"
    fi

    echo
  fi

  stat $f | grep -E 'Change|Birth' | sort | cut -c-30
  echo
done
