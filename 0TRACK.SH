#!/bin/bash

params=$*
digits=""

while getopts "d:" opt
do
	case ${opt} in
		d) check=$(echo "${OPTARG}" | tr -d "[0-9]"); if [ "${check}" = "" ]; then digits="${OPTARG}"; fi ;;
	esac
done

prevparam=""
for param in ${params} ; do
  if [ "${param}" != "" ]; then
    if [ "${digits}" = "" ]; then
      check=$(echo "${param}" | tr -d "[0-9]")
      if [ "${check}" = "" -a "${prevparam}" != "-d"  -a "${prevparam}" != "-s" ]; then digits=${param}; fi
    fi
    prevparam=${param}
  fi
done

if [ "${digits}" = "" ]; then echo "Missing digits parameter"; exit 0; fi

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

echo > running.txt
(./0RUNME.SH $* &)

prev=""
while true ; do
  if [ ! -e running.txt -a ! -e RUNNING.TXT ]; then echo Done; exit; fi

  size=""
  what=""
  proc=$($PSCMD | grep -E "giopi|giotst" | tr -s " " | tail -1)
  if [ "$proc" != "" ]; then
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
