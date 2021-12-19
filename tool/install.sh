#!/bin/bash

if ! command -v gcc &> /dev/null
then
	echo -e "Please install the Gnu Compiler Collection (gcc)"
	exit
else
	echo -e "Compiling JamfEdit..."
	gcc jamfedit.c -o jamfedit
	if [ $? -eq 0 ]
	then
		clear
		if [ -f "/usr/sbin/jamfedit" ]
		then
			echo "Overwriting previous installation"
			sudo rm /usr/sbin/jamfedit
		fi
		echo "Linking $(pwd)/jamfedit to /usr/sbin/ ..."
		sudo ln -s $(pwd)/jamfedit /usr/sbin/jamfedit
		if [ $? -eq 0 ] 
		then
			echo -e "\033[1;33mSuccessfully installed JamfEdit\033[m"
		else
			echo -e "Linking failed"
		fi
	else
		echo -e "Could not compile!"
	fi
fi
