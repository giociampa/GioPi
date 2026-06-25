#!/bin/bash

find -maxdepth 1 -iname "running.txt" | while read f ; do
  printf "Runner: %s\n" "$(tail -1 $f)"
  echo
done

ls -1 *.run *.RUN 2>/dev/null | while read f ; do
  echo "  File: $f"
  stat $f | grep -E 'Change|Birth' | cut -d. -f1 | sort
  printf "Status: %s\n" "$(tail -1 $f)"
  echo
done

grep -H Total *.log *.LOG 2>/dev/null | while read l ; do
  f=$(echo $l | cut -d: -f1)
  d=$(echo $l | cut -d: -f2-)
  t=$(echo $f | sed -e "s/log/txt/" -e "s/LOG/TXT/")
  r=$(echo $f | sed -e "s/log/raw/" -e "s/LOG/RAW/")
  echo $f
  if [ -e "$r" -a ! -e "$t" ]; then 0raw2txt $r ; fi
  if [ -e "$t" ]; then 0compare ~/1billion.pi $t | head -1 ; fi
  echo $d
  echo
done
