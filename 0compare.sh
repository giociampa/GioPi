ls -1 *.txt 2>/dev/null | sort -n | grep -v README | while read f; do
    echo $f
    ~/Source/gio-pi/compare ~/1billion.pi $f | head -1
    grep Total ${f%txt}log
    echo
done
