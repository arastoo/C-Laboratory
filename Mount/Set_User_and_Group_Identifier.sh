#!/bin/bash

rootCheck () {
    if [ $(id -u) != "0" ]
    then
		printf "\nErr: %s should run as root\n" $0
        exit 1
    fi
}

setUid () {
	if [[ $1 -eq 1 ]]
	then
		if [[ ! -e $2 ]]
		then
			printf "\nError: File doesn't exist.\n"
			exit 1
		fi
		if [ -x $2 ] && file $2 | grep -q "GNU/Linux"
		then
			chown root:root $2
			chmod u+s $2
			chmod g+s $2
			exit 0
		fi
	else
		printf "\nError\n"
		exit 1
	fi
}

main () {
	rootCheck
	setUid $1 $2
}

main $# $@
