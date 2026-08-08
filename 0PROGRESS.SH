#!/bin/bash

ONEPASS=""
NUMLINE=1
for PARAM in $* ; do
  TEST=$(echo "$PARAM" | grep -i -E "ONCE|ONEPASS")
  if [ "$TEST" != "" ]; then ONEPASS="ONEPASS"; fi

  TEST=$(echo ":$PARAM:" | tr -d "[0-9]")
  if [ "$TEST" = "::" ]; then NUMLINE=$PARAM; fi
done

ARANYM=$(uname -a | grep -i ARANYM)
if [ "$ARANYM" = "" ]; then
  ARANYM=$(echo $PWD | grep -i ARANYM)
fi
if [ "$ARANYM" = "" ]; then
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
if [ "$ZEDFILE" = "NONE" ]; then
  echo "No zedfile found"
  exit
fi

while true ; do
	if [ "$ONEPASS" = "" ]; then clear; fi
	line=0
	last=$(tail -1 $ZEDFILE | grep "Finished")
	for f in $FILELIST ; do
    if [ $line -eq 1 ]; then echo; fi
		n=$(echo $f | cut -d. -f1)
		for l in Split Combine Write ; do
			sort $ZEDFILE -n | grep $n | grep $l | tail -$NUMLINE | while read SORTED; do
        echo "$(echo $SORTED | cut -d\  -f-2) $(echo $SORTED | cut -d\  -f4-)"
			done
      if [ $NUMLINE -gt 1 ]; then echo; fi
		done
		echo
    sort $ZEDFILE -n -k 3 | grep $n | tail -1 | cut -d\  -f2-
    line=1
	done
	if [ "$ONEPASS" != "" -o "$last" != "" ]; then exit 0; fi
	sleep 5
done
