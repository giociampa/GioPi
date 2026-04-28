digits=$1
shift
extras=$*

function process () {
  local runthis=$1
  if [ -e "${runthis}" ]; then
    stripped=${runthis%.ttp}
    running=${digits}-${stripped}
    echo "Running: ${stripped} ${digits} ${extras}" | tee ${running}.running
    ./${runthis} ${digits} ${extras} 2>&1
    for ext in log raw run txt ; do
      if [ -e "${digits}.${ext}" ]; then mv "${digits}.${ext}" "${running}.${ext}"; fi
      if [ -e "pi.${ext}" ]; then mv "pi.${ext}" "${running}.${ext}"; fi
    done
    rm -f ${running}.running
    echo
  fi
}

rm -f *running*
for source in pi pitst ; do process "${source}" ; done
