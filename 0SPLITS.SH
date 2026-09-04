#!/bin/bash

for split in 2 4 8 ; do
  ./0TRACK.SH $* split$split
  mv zzzz.txt zzsplit$split.txt
  sed -i "s/$/ ($split)/" zzsplit$split.txt
done
