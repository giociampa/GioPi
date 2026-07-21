#!/bin/bash

PAR=$*
NUM=0
RAW=""
OUT="TXT"

for TEST in ${PAR} ; do
  if [ "$(echo ${TEST} | tr -d '[0-9]')" = "" ]; then NUM=${TEST} ; fi
  if [ "$(echo ${TEST} | grep -i RAW)" != "" ]; then OUT="RAW" ; RAW="RAW" ; fi
done

if [ ${NUM} -lt 1000 ]; then NUM=1000 ; fi

ls -1 *.PRG *.TTP 2>/dev/null | while read FILE ; do
  NAM=$(echo ${FILE} | cut -d\. -f1)
  EXT=$(echo ${FILE} | cut -d\. -f2)

  echo "ECHO Running ${FILE} ${NUM} ${RAW}"
  echo "ECHO Running ${FILE} ${NUM} ${RAW} > RUNNING.TXT"
  echo "${FILE} ${NUM} ${RAW}"
  for TYP in LOG ${OUT} ; do
    echo REN \"${NUM}.${TYP}\" \"$(echo "${NUM}" | sed "s/000//")$(echo "${NAM}" | sed "s/GIO//").${TYP}\"
  done
  echo "DEL RUNNING.TXT"
  echo
done | tee 0RUNME.BAT

unix2dos 0RUNME.BAT 1>/dev/null 2>/dev/null
chmod +x 0RUNME.BAT 1>/dev/null 2>/dev/null
