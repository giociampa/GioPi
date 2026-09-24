#!/bin/bash

rm -f zzsplit*.txt zzzz.txt

for split in 1 2 4 8 ; do
  ./0TRACK.SH $* -$split
  mv zzzz.txt zzsplit$split.txt
  sed -i "s/$/ ($split)/" zzsplit$split.txt
done

cat zzsplit*.txt > zzzz.txt
rm -f zzsplit*.txt
