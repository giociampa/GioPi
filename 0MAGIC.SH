#!/bin/bash

0uppercase doit 1>/dev/null 2>/dev/null

for oldfile in GIO*TTP; do
  newfile=$(echo $oldfile | sed -e "s/PI/P/" -e "s/TST/T/" -e "s/0//g")
  if [ "$oldfile" != "$newfile" ] ; then
    execute="mv \"$oldfile\" \"$newfile\""
    echo $execute | sh
    echo $execute
  fi
done 2>/dev/null
