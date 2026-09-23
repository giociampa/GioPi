#!/bin/bash

params=$*
digits=""
splits="2"
dozing="10"

while getopts "d:248" opt
do
	case ${opt} in
		d) check=$(echo "${OPTARG}" | tr -d "[0-9]"); if [ "${check}" = "" ]; then digits="${OPTARG}"; fi ;;
		2) splits="2" ;;
		4) splits="4" ;;
		8) splits="8" ;;
	esac
done

prevparam=""
for param in ${params} ; do
  if [ "${param}" != "" ]; then
    if [ "${digits}" = "" ]; then
      check=$(echo "${param}" | tr -d "[0-9]")
      if [ "${check}" = "" -a "${prevparam}" != "-d"  -a "${prevparam}" != "-s" ]; then digits=${param}; fi
    fi
    check=$(echo "${param}" | grep -i "^sleep=")
    if [ "${check}" != "" ]; then
      check=$(echo "${param}" | cut -d= -f2 | tr -d "[0-9]")
      if [ "${check}" = "" ]; then dozing=$(echo "${param}" | cut -d= -f2); fi
    fi
    prevparam=${param}
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

    echo "Running: ${stripped} ${params}" | tee running.txt
    ./${runthis} ${params}

    for ext in log raw run txt ; do
      if [ -e "${digits}.${ext}" ]; then mv "${digits}.${ext}" "${running}.${ext}"; fi
      if [ -e "pi.${ext}" ]; then mv "pi.${ext}" "${running}.${ext}"; fi
    done

    for part in p q t ; do
      if [ -e "pass-9-${part}.tmp" ]; then mv "pass-9-${part}.tmp" "${running}-9-${part}.tmp"; fi
    done

    echo "Finished: ${stripped} ${params}" | tee running.txt
    echo
  fi
}

echo > running.txt
firstone=1
for source in $FILELIST ; do
  if [ $firstone -eq 1 ]; then
    firstone=0
  else
    echo "Sleeping:" | tee running.txt
    sleep "${dozing}"
    echo
  fi
  process "${source}"
done
rm -f running.txt 2>/dev/null
