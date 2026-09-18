#!/bin/bash

ONCE=""
TIME=5
for param in $* ; do
  if [ "$(echo $param | grep -i once)" != "" ]; then ONCE="ONCE"; fi
  if [ "$(echo $param | tr -c -d '[0-9]')" = "${param}" ]; then TIME=${param}; fi
done

done=0
while [ $done -eq 0 ]; do
  clear

  runfile=""
  for testing in "running.txt" "RUNNING.TXT"; do
    if [ -e "${testing}" ]; then
      runfile=$(for d in $(cat "${testing}"); do if [ -e "${d}.run" ]; then echo "${d}.run"; fi; done)
      if [ "${runfile}" != "" ]; then echo $(cat "${testing}"); echo $(stat ${runfile} | grep Change | cut -c-30 | sed "s/e/ed/"); echo $(cat ${runfile}); fi
    fi
  done
  if [ "${runfile}" = "" ]; then echo "Running: None"; done=1; fi

  echo
  for logfile in *.log ; do
    runfile="${logfile%log}run"
    txtfile="${logfile%log}txt"
    seefile=""
    if [ -e "$txtfile" ]; then
      seefile="${txtfile}"
    elif [ -e "$runfile" ]; then
      seefile="${runfile}"
    fi
    if [ "${seefile}" != "" ]; then printf "%-25s  %'15d  %s\n" "${seefile}" "$(du -b ${seefile} | cut -f1)"  "$(stat ${seefile} | grep Change | cut -d\  -f2- | cut -d\. -f1)" ; fi
  done

  printf "%-25s  %'15d\n" "(Temp. Storage)" "$(du -b pass* PASS* 2>/dev/null | awk 'BEGIN{sum=0}{sum+=$1}END{print sum}')"

  if [ "$ONCE" = "ONCE" ]; then done=1; fi
  if [ $done -eq 0 ]; then sleep $TIME; fi
done
