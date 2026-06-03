#!/bin/bash

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi

if [ "$aranym" = "" ]; then
  FILELIST="giopi giotst"
else
  FILELIST="gio*.ttp gio*.prg"
fi

for f in $FILELIST ; do
	echo $f
	strings -a $f | grep -E "GCC|MiNTLib"
	echo
done
