#!/bin/bash

D=${1:-1000}

ls -1 *.PRG *.TTP 2>/dev/null | while read F ; do
  N=$(echo $F | cut -d\. -f1)
  E=$(echo $F | cut -d\. -f2)
  R="$N.RUN"
  echo "ECHO $F $D > RUNNING.TXT"
  echo $F $D
  for X in LOG TXT; do
    echo "REN $D.$X $(echo $N | sed -e 's/gio//' -e 's/GIO//')$E.$X"
  done
  echo "DEL RUNNING.TXT"
  echo
done | tee 0RUNME.BAT

unix2dos 0RUNME.BAT
