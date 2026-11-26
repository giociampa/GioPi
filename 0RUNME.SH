#!/bin/bash

params=$*
digits=""
splits="2"
dozing=""

while getopts "d:s:1248" opt
do
	case ${opt} in
		d) if [ -z "$(echo ${OPTARG} | tr -d '[0-9]')" ]; then digits=${OPTARG}; fi ;;
		s) if [ -z "$(echo ${OPTARG} | tr -d '[0-9]')" ]; then dozing=${OPTARG}; fi ;;
		1) splits="1" ;;
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

if [ -z "${digits}" ]; then echo "Missing digits parameter"; exit 0; fi

aranym=$(uname -a | grep -i aranym)

if [ "$aranym" = "" ]; then
  aranym=$(echo $PWD | grep -i aranym)
fi
if [ "$aranym" = "" ]; then
  FILELIST="giopi giotst chudpi testpi"
else
  FILELIST="gio*.ttp chudpi.ttp testpi.ttp"
fi

function process () {
  local runthis=$1
  if [ -e "${runthis}" ]; then
    stripped="${runthis%.ttp}"
    running="${digits}-${stripped}-${splits}"

    echo "Running: ${stripped} ${params}" | tee running.txt
    ./${runthis} ${params}

    for ext in log raw run tmp txt ; do
      if [ -e "${digits}.${ext}" ]; then mv "${digits}.${ext}" "${running}.${ext}"; fi
      if [ -e "pi.${ext}" ]; then mv "pi.${ext}" "${running}.${ext}"; fi
    done

    for part in p q t ; do
      if [ -e "pass-9-${part}.tmp" ]; then mv "pass-9-${part}.tmp" "${running}-9-${part}.tmp"; fi
    done

    if [ -e "ZZZDEBUG.TXT" ]; then mv "ZZZDEBUG.TXT" "${running}-ZZZDEBUG.TXT"; fi

    echo "Finished: ${stripped} ${params}" | tee running.txt
    echo
  fi
}

echo > running.txt
firstone=1
for source in $FILELIST ; do
  if [ $firstone -eq 1 ]; then
    firstone=0
  elif [ ! -z "${dozing}" ]; then
    echo "Sleeping:" | tee running.txt
    sleep "${dozing}"
    echo
  fi
  process "${source}"
done
rm -f running.txt 2>/dev/null
