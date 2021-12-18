#!/bin/bash

if [ -z $1 ]
then
	echo "No argument supplied"
else
	echo -e "Compiling $1..."
	gcc $1
	if [ $? -eq 0 ]
	then
		echo "Executing..."
		./a.out
	fi
fi
