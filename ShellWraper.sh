#!/bin/sh
reset-mcu
STR=$(cat /tmp/datalog.txt)
STD=$(cat /tmp/datalog.txt)

LIMIT=20
i=0
while [ $i -lt "$LIMIT" ]; do
	sleep 1
	i=$(($i+1))
	STD=$(cat /tmp/datalog.txt)
	if [ "$STD" != "$STR" ]; then
		weather_sql.py
		break
	fi
done
