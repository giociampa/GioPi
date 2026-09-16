#!/bin/bash

rm -f zzsplit2 zzsplit4 zzsplit8 zzzz.txt

for split in 2 4 8 ; do
  ./0TRACK.SH $* split$split
  mv zzzz.txt zzsplit$split.txt
  sed -i "s/$/ ($split)/" zzsplit$split.txt
done

cat zzsplit*.txt > zzzz.txt
rm -f zzsplit*.txt
