( ./0runme.sh $* ) &

digits=$1
flag=""
prev=""

while true ; do
  proc=$(ps -eo size,cmd | grep "/pi" | grep -v grep | grep -v pipe | grep -v tee | tr -s " ")
  if [ "$proc" = "" ]; then
    if [ "$flag" = "flag" ]; then
      exit
    fi
  else
    if [ "$(echo "$proc" | grep -i pitst)" != "" ]; then
      flag="flag"
    fi

    if [ -e "${digits}.run" ]; then
      line=$(cat ${digits}.run | tr -d "[:cntrl:]")
    else
      line=""
    fi

    size=$(echo $proc | cut -d\  -f1)
    what=$(basename $(echo $proc | cut -d\  -f2))
    this="$size $what $line"
    if [ "$this" != "$prev" ]; then
      echo "$this" >> zzzz.txt
      prev="$this"
    fi
  fi
done
