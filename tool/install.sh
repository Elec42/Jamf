#!/bin/bash

if ! command -v gcc &> /dev/null
then
	echo -e "Please install the Gnu Compiler Collection (gcc)"
	exit
else
	echo -e "Compiling JamfEdit..."
	gcc jamfedit.c -o jamfeditFSG
	if [ $? -eq 0 ]
	then
		if [ -f "/usr/sbin/jamfedit" ]
		then
			echo "Overriding previous install"
			sudo rm /usr/sbin/jamfeditFSG
		fi
		echo "Linking $(pwd)/jamfedit to /usr/sbin/ ..."
		sudo ln -s $(pwd)/jamfeditFSG /usr/sbin/jamfeditFSG
	fi
fi
