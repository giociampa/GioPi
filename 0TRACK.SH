#!/bin/bash

digits=$1
runfile=${digits}.run
logfile=${digits}.log

rm -f zzzz.txt ZZZZ.TXT

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi

if [ "$aranym" = "" ]; then
  PSCMD="ps -eo size,comm"
  PSIZE=1
  PNAME=2
else
  PSCMD="ps"
  PSIZE=6
  PNAME=8
fi


echo > RUNNING.TXT
(./0RUNME.SH $* &)

flag=""
prev=""
while true ; do
  size=""
  what=""

  if [ "$(ls -1 RUNNING.TXT 2>/dev/null | wc -l)" = "0" ]; then
    if [ "$flag" = "flag" ]; then
      echo "Done"
      exit 0;
    fi
  fi

  proc=$($PSCMD | grep -E "giopi|giotst" | tr -s " " | tail -1)
  if [ "$proc" != "" ]; then
    if [ "$(echo $proc | grep giotst)" = "" ]; then flag="flag"; fi
    size=$(echo $proc | cut -d\  -f$PSIZE)
    what=$(basename $(echo $proc | cut -d\  -f$PNAME))
    pass=$(du -b pass* 2>&1 | awk 'BEGIN{sum=0}{sum+=$1}END{print sum}')

    if [ -e $runfile ]; then
      line=$(tail -1 $runfile 2>/dev/null | tr -s " " | tr -d "[:cntrl:]" )
    elif [ -e $logfile ]; then
      line=$(tail -1 $logfile 2>/dev/null | tr -s " " | tr -d "[:cntrl:]" )
    else
      line=""
    fi

    this="$size $what $pass $line"
    if [ "$this" != "$prev" ]; then
      echo "$this" >> zzzz.txt
      prev="$this"
    fi
  fi
done
