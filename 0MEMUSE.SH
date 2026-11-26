what=$1

done=0
most=0
when=""
skip=1
while true ; do
  line=$(ps -eo size,comm | grep $what | grep -v grep)
  if [ "$skip" = "0" -a "$line" = "" ]; then
    exit
  elif [ "$line" != "" ]; then
    skip=0
    size=$(( $(echo $line | cut -d\  -f1) + 0 ))
    if [ $size -gt $most ]; then most=$size; when=$(cat *.run 2>/dev/null); fi
    printf "%10s  %10s  %s\n" "$size"  "$most" "$when"
  fi
  sleep 1
done
