#!/bin/bash

WINDOW="hann"

usage () {
  echo "                                                                        " >&2
  echo "NAME                                                                    " >&2
  echo "  sqcrosscorr - cross-correlates two signal files                       " >&2
  echo "SYNOPSIS                                                                " >&2
  echo "  sqcrosscorr [OPTIONS] file1 file2                                     " >&2
  echo "OPTIONS                                                                 " >&2
  echo "  -l FFT length integer (required)                                      " >&2
  echo "  -w window type [wola, hann]; default is wola                          " >&2
  echo "  -h show help (this)                                                   " >&2
  echo "EXAMPLE                                                                 " >&2
  echo "  sqcrosscorr -c 1420.0 time-frequency-power.dat                        " >&2
  echo "                                                                        " >&2
}

while getopts hl:w: OPTION
do
    case $OPTION in 
        l) length=$OPTARG;;
        w) WINDOW=$OPTARG;;
        h) usage && exit 1;;
        ?) usage && exit 1;;
    esac
done

# Choose block
if [ "$WINDOW" == "wola" ]; then
    WINDOW_BLOCK="sqwola -f 9 -o 0"
else
    WINDOW_BLOCK="sqwindow -w $WINDOW"
fi

mkfifo fft1
mkfifo fft2

cat $5 | sqsample -l $length | $WINDOW_BLOCK -l $length | sqfft -l $length > fft1 &
cat $6 | sqsample -l $length | $WINDOW_BLOCK -l $length | sqfft -l $length | sqconjugate -l $length > fft2 &

sqcrossmultiply fft1 fft2 -l $length | sqfft -l $length -i

rm fft1 fft2
