#!/bin/bash

PAR=$*
NUM=0
EXT="TXT"

for TEST in $PAR ; do
  if [ "$(echo $TEST | tr -d '[0-9]')" = "" ]; then NUM=$TEST ; fi
  if [ "$(echo $TEST | grep -i RAW)" != "" ]; then EXT="RAW" ; fi
done

if [ $NUM -eq 0 ]; then NUM=1000 ; fi

ls -1 *.PRG *.TTP 2>/dev/null | while read F ; do
  NAM=$(echo ${F} | cut -d\. -f1)
  E=$(echo ${F} | cut -d\. -f2)

  echo "ECHO $F $PAR > RUNNING.TXT"
  echo $F $PAR
  for TYP in LOG $EXT ; do
    echo "REN $NUM.$TYP $(echo $NAM | sed -e 's/gio//' -e 's/GIO//')$E.$TYP"
  done
  echo "DEL RUNNING.TXT"
  echo
done | tee 0RUNME.BAT

unix2dos 0RUNME.BAT
chmod +x 0RUNME.BAT