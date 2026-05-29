#!/bin/bash

if [ -e README.txt ]; then
  mv README.txt README.txt.000
fi

rm -f *.dat *.err *.log *.txt *.raw *.tee *.tmp *.run *.running

if [ -e README.txt.000 ]; then
  mv README.txt.000 README.txt
fi
