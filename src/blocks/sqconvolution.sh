#!/bin/bash

while getopts l: OPTION
do
    case $OPTION in 
        l) length=$OPTARG;;
        \?) print -u2 "Usage: $0 <file1> <file2> [ -l points ]"
        exit 2;;
    esac
done
mkfifo fft1
mkfifo fft2

length=4096

cat $1 | ./sqsample -l $length | ./sqwindow -l $length | ./sqfft -l $length > fft1 &
cat $2 | ./sqsample -l $length | ./sqwindow -l $length | ./sqfft -l $length > fft2 &

./sqcrossmultiply fft1 fft2 -l $length | ./sqfft -l $length -i 

rm fft1 fft2
