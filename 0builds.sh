#!/bin/bash

aranym=$(uname -a | grep -i aranym)
if [ "$aranym" = "" ]; then
  FILELIST="giopi giotst"
else
  FILELIST="gio*.ttp"
fi

for f in $FILELIST ; do
	echo $f
	strings -a $f | grep -E "GCC|MiNTLib"
	echo
done
