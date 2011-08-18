#!/bin/bash

#!/bin/bash

WINDOW="wola"

usage () {
  echo "                                                                        " >&2
  echo "NAME                                                                    " >&2
  echo "  sqautocorr - performs autocorrelation of a signal                     " >&2
  echo "SYNOPSIS                                                                " >&2
  echo "  sqautocorr [OPTIONS]                                                  " >&2
  echo "OPTIONS                                                                 " >&2
  echo "  -l FFT length integer (required)                                      " >&2
  echo "  -w window type [wola, hann]; default is wola                          " >&2
  echo "  -h show help (this)                                                   " >&2
  echo "EXAMPLE                                                                 " >&2
  echo "  sqautocorr -l 4096 -w hann time-frequency-power.dat                   " >&2
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

cat | $WINDOW_BLOCK -l $length | sqfft -l $length | sqpower -l $length | sqfft -l $length -i

