#!/bin/bash

once=$(echo "$1" | grep -i once)

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi

if [ "$aranym" = "" ]; then
  FILELIST="giopi giotst"
else
  FILELIST="gio*.ttp"
fi

while true ; do
	if [ "$once" != "" ]; then exit 0; fi
	clear
	last=$(tail -1 zzzz.txt)
	sort -n zzzz.txt | tail -1
	echo
	for f in $FILELIST ; do
		n=$(echo $f | cut -d. -f1)
		sort -n zzzz.txt | grep $n | tail -1
		for l in Split Combine Write Total ; do
			sort zzzz.txt -n | grep $n | grep $l | tail -1
		done
		echo
	done
	if [ "$once" != "" -o "$last" = "0 Finished" ]; then exit 0; fi
	sleep 5
done
