#!/bin/bash

if [[ -z $1 ]] || [[ -z $2 ]] ; then
	echo "Usage: $0 <number> <team-name>"
	exit 0
fi

for ((i = 1 ; i <= $1 ; i++)); do
	./run -n $2 -p 9876 > /dev/null &
done

echo "run $1 client(s)"
