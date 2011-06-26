#!/bin/bash

while getopts l: OPTION
do
    case ${OPTION} in
        l) length=${OPTARG};;
       \?) print -u2 "Usage: ${PROG_NAME} [ -l points ] [data]"
           exit 2;;
    esac
done

cat | ./sqwindow -l $length | ./sqfft -l $length | ./sqpower -l $length | ./sqfft -l $length -i 

