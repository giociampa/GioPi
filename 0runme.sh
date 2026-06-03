#!/bin/bash

params=$*
digits=$1

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi

if [ "$aranym" = "" ]; then
  FILELIST="giopi giotst"
else
  FILELIST="gio*.ttp gio*.prg"
fi

function process () {
  local runthis=$1
  if [ -e "${runthis}" ]; then
    stripped=${runthis%.ttp}
    running=${digits}-${stripped}
    echo "Running: ${stripped} ${params}"
    ./${runthis} ${params} 2>&1 | tee ${running}.running
    for ext in log raw run txt ; do
      if [ -e "${digits}.${ext}" ]; then mv "${digits}.${ext}" "${running}.${ext}"; fi
      if [ -e "pi.${ext}" ]; then mv "pi.${ext}" "${running}.${ext}"; fi
    done
    rm -f ${running}.running
    echo
  fi
}

rm -f *running*
for source in $FILELIST ; do process "${source}" ; done
