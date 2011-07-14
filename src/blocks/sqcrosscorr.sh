#!/bin/bash

while getopts l: OPTION
do
    case $OPTION in 
        l) length=$OPTARG;;
        \?) print -u2 "Usage: $0 [ -l points ] <file1> <file2>"
           exit 2;;
    esac
done

mkfifo fft1
mkfifo fft2

cat $3 | sqsample -l $length | sqwindow -l $length | sqfft -l $length > fft1 &
cat $4 | sqsample -l $length | sqwindow -l $length | sqfft -l $length | sqconjugate -l $length > fft2 &

sqcrossmultiply fft1 fft2 -l $length | sqfft -l $length -i

rm fft1 fft2
