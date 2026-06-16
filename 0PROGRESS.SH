#!/bin/bash

once=$(echo "$1" | grep -i once)

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi

if [ "$aranym" = "" ]; then
  FILELIST="giopi giotst"
else
  FILELIST="gio*.ttp gio*.prg"
fi

while true ; do
	if [ "$once" = "" ]; then clear; fi
	last=$(tail -1 zzzz.txt)
	for f in $FILELIST ; do
		n=$(echo $f | cut -d. -f1)
		for l in Split Combine Write Total ; do
			sort zzzz.txt -n | grep $n | grep $l | tail -5
      echo
		done
	done
	if [ "$once" != "" -o "$last" = "0 Finished" ]; then exit 0; fi
	sleep 5
done
