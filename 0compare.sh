#!/bin/bash

grep Result *.log | sort -n | grep -v README | cut -d: -f1 | while read f ; do
	r=${f%.log}.txt
	echo $f
	grep Total $f
	if [ -e "$r" ]; then
		./compare ~/1billion.pi "$r" | head -n 1
	fi
	echo
done
