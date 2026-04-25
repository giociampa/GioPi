touch yyyy.txt
prev=""
while true ; do
  proc=$(ps -eo size,cmd | grep "/pi" | grep -v wire | grep -v grep)
  if [ "$proc" != "" ]; then
    line=$(tail -1 yyyy.txt | tr -d "[:cntrl:]" | cut -d: -f1)
    this="$proc $line"
    if [ "$this" != "$prev" ]; then
      echo "$this"
      prev="$this"
    fi
  fi
done | tee zzzz.txt
