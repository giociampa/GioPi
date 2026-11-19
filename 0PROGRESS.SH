#!/bin/bash

SLEEP="5"
NUMLINE="3"
ONEPASS=""
ALLFILT="Split Combine Root Divide Write"
USEFILT=""
SEETEMP="SEETEMP"

while getopts "s:n:t:f:10?" opt
do
  case ${opt} in
    s) if [ -z "$(echo ${OPTARG} | tr -d '[0-9]')" ]; then SLEEP=${OPTARG}; fi ;;
    n) if [ -z "$(echo ${OPTARG} | tr -d '[0-9]')" ]; then NUMLINE=${OPTARG}; fi ;;
    t) if [ -z "${OPTARG}" ] || [ "${OPTARG}" -gt 0 ]; then SEETEMP="SEETEMP"; else SEETEMP=""; fi;;
    f) USEFILT="${USEFILT} ${OPTARG}";;
    1) ONEPASS="ONEPASS";;
    0) ONEPASS="";;
    ?) echo "Usage: $0 [-s sleep] [-n NUMLINE] [-f filter] [-1|-0]"; exit 1;;
  esac
done

if [ "${USEFILT}" = "" ]; then USEFILT="${ALLFILT}"; fi

ARANYM=$(uname -a | grep -i ARANYM)
if [ "${ARANYM}" = "" ]; then
  ARANYM=$(echo $PWD | grep -i ARANYM)
fi
if [ "${ARANYM}" = "" ]; then
  FILELIST="giopi giotst"
else
  FILELIST="gio*.ttp"
fi

ZEDFILE="NONE"
for TEST in zzzz.txt ZZZZ.TXT; do
  if [ -e $TEST ]; then
    SIZE=$(du -b $TEST | cut -f1)
    if [ $SIZE -gt 0 ]; then ZEDFILE=$TEST; fi
  fi
done
if [ "${ZEDFILE}" = "NONE" ]; then
  echo "No zedfile found"
  exit
fi

while true ; do
  if [ "${ONEPASS}" = "" ]; then clear; fi
  line=0
  last=$(tail -n 1 ${ZEDFILE} | grep "Finished")
  for f in $FILELIST ; do
    if [ $line -eq 1 ]; then echo; fi
    n=$(echo $f | cut -d. -f1)
    printf "%s %s\n" "$n" "$(grep Split *${f}*.log | cut -d\  -f2)"
    for l in ${USEFILT} ; do
      sort ${ZEDFILE} -n | uniq -u | grep $n | grep -i $l | tail -n ${NUMLINE} | while read SORTED; do
        echo "$(echo ${SORTED} | cut -d\  -f1) $(echo ${SORTED} | cut -d\  -f4-)"
      done
      if [ ${NUMLINE} -gt 1 ]; then echo; fi
    done
    if [ "${SEETEMP}" = "SEETEMP" ]; then
      ramuse=$(sort ${ZEDFILE} -n | grep $n | tail -n 1)
      printf "%s %s\n" "$(echo ${ramuse} | cut -d\  -f1)" "$(echo ${ramuse} | cut -d\  -f4-)"
      tmpuse=$(sort ${ZEDFILE} -n -k 3 | grep $n | tail -n 1)
      printf "%s\n" "$(echo ${tmpuse} | cut -d\  -f3-)"
    fi
    line=1
  done
  if [ "${ONEPASS}" != "" -o "$last" != "" ]; then exit 0; fi
  sleep ${SLEEP}
done

