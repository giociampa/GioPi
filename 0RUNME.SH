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
    echo "Running: ${stripped} ${params}" 2>&1 | tee RUNNING.TXT
    ./${runthis} ${params}
    rm -f RUNNING.TXT
    for ext in log run raw txt ; do
      if [ "${ext}" = "raw" ]; then
        if [ -e "${digits}.txt" ]; then ./rawtxt "${digits}.${ext}"; fi
      else
        mv "${digits}.${ext}" "${running}.${ext}"
      fi
      if [ -e "pi.${ext}" ]; then mv "pi.${ext}" "${running}.${ext}"; fi
    done
    echo
  fi
}

rm -f *running* *RUNNING* 2>/dev/null
for source in $FILELIST ; do process "${source}" ; done
