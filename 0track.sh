#!/bin/bash

rm -f zzzz.txt
touch zzzz.txt

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi

if [ "$aranym" = "" ]; then
  PSCMD="ps -eo size,cmd"
  PSIZE=1
  PNAME=2
else
  PSCMD="ps"
  PSIZE=6
  PNAME=8
fi

( ./0runme.sh $* ) &

digits=$1
flag=""
prev=""

while true ; do
  proc=$($PSCMD | grep -E "giopi|giotst" | grep -v grep | grep -v tee | tr -s " ")
  if [ "$proc" = "" ]; then
    if [ "$flag" = "flag" ]; then
      echo "0 Finished" >> zzzz.txt
      exit
    fi
  else
    test=$(echo "$proc" | grep -i tst)
    if [ "$test" != "" ]; then
      flag="flag"
    fi

    if [ -e "${digits}.run" ]; then
      line=$(cat ${digits}.run | tr -d "[:cntrl:]")
    else
      line=""
    fi

    size=$(echo $proc | cut -d\  -f$PSIZE)
    what=$(basename $(echo $proc | cut -d\  -f$PNAME))
    this="$size $what $line"
    if [ "$this" != "$prev" ]; then
      echo "$this" >> zzzz.txt
      prev="$this"
    fi
  fi
done
