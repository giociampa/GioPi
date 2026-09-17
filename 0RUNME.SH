#!/bin/bash

params=$*
digits=""
splits="2"

for param in ${params} ; do
  if [ "${param}" != "" ]; then
    check=$(echo "${param}" | tr -d "[0-9]")
    if [ "$check" = "" ]; then digits=${param}; fi
    
    check=$(echo "${param}" | grep -i -E "split2|split4|split8")
    if [ "$check" != "" ]; then splits=$(echo "${param}" | tr "[:lower:]" "[:upper:]" | sed "s/SPLIT//"); fi
  fi
done

if [ "${digits}" = "" ]; then echo "Missing digits parameter"; exit 0; fi

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi

if [ "$aranym" = "" ]; then
  FILELIST="giopi giotst"
else
  FILELIST="gio*.ttp"
fi

function process () {
  local runthis=$1
  if [ -e "${runthis}" ]; then
    stripped="${runthis%.ttp}"
    running="${digits}-${stripped}-${splits}"

    echo "Running: ${stripped} ${params}" 2>&1 | tee running.txt
    ./${runthis} ${params}
    echo "Finished: ${stripped} ${params}" 2>&1 | tee running.txt

    for ext in log raw run txt ; do
      if [ -e "${digits}.${ext}" ]; then mv "${digits}.${ext}" "${running}.${ext}"; fi
      if [ -e "pi.${ext}" ]; then mv "pi.${ext}" "${running}.${ext}"; fi
    done

    for part in p q t ; do
      if [ -e "pass-9-${part}.tmp" ]; then mv "pass-9-${part}.tmp" "${running}-9-${part}.tmp"; fi
    done

    echo
  fi
}

echo > running.txt 2>/dev/null
for source in $FILELIST ; do process "${source}" ; done
rm -f running.txt 2>/dev/null
