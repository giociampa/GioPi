#!/bin/bash

if [ -e README.txt ]; then
  mv README.txt README.txt.000
fi

rm -f *.dat *.err *.log *.txt *.raw *.tee *.tmp *.run zzzz.txt
rm -f *.DAT *.ERR *.LOG *.TXT *.RAW *.TEE *.TMP *.RUN ZZZZ.TXT

if [ -e README.txt.000 ]; then
  mv README.txt.000 README.txt
fi
