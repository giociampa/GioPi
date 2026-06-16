#!/bin/bash

digits=$1
runfile=${digits}.run
logfile=${digits}.log

rm -f yyyy.txt zzzz.txt YYYY.TXT ZZZZ.TXT
touch yyyy.txt zzzz.txt YYYY.TXT ZZZZ.TXT

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

(./0RUNME.SH $* &)

flag=""
prev=""

while true ; do
  proc=$($PSCMD | grep -E "giopi|giotst" | grep -v grep | grep -v tee | tr -s " " | tail -1)
  if [ "$flag" = "" -a "$(echo $proc | grep giotst)" != "" ]; then
    flag="flag"
  fi
  if [ "$proc" = "" -a "$flag" = "flag" ]; then
    echo "Done"
    exit 0
  fi

  if [ -e $runfile ]; then
    line=$(tail -1 $runfile 2>/dev/null | tr -s " " | tr -d "[:cntrl:]" )
  elif [ -e $logfile ]; then
    line=$(tail -1 $logfile 2>/dev/null | tr -s " " | tr -d "[:cntrl:]" )
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
done
