#!/bin/bash

PAR=$*
NUM=0
TYP="TXT"

for TEST in ${PAR} ; do
  if [ "$(echo ${TEST} | tr -d '[0-9]')" = "" ]; then NUM=${TEST} ; fi
  if [ "$(echo ${TEST} | grep -i RAW)" != "" ]; then TYP="RAW" ; fi
done

if [ ${NUM} -eq 0 ]; then NUM=1000 ; fi

ls -1 *.PRG *.TTP 2>/dev/null | while read FILE ; do
  NAM=$(echo ${FILE} | cut -d\. -f1)
  EXT=$(echo ${FILE} | cut -d\. -f2)

  echo "ECHO Running ${FILE} ${PAR}"
  echo "ECHO ${FILE} ${PAR} > RUNNING.TXT"
  echo ${FILE} ${PAR}
  for TYP in LOG ${TYP} ; do
    echo "REN ${NUM}.${TYP} $(echo ${NAM} | sed -e 's/gio//' -e 's/GIO//')${EXT}.${TYP}"
  done
  echo "DEL RUNNING.TXT"
  echo
done | tee 0RUNME.BAT

unix2dos 0RUNME.BAT
chmod +x 0RUNME.BAT