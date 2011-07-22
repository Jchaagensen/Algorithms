#!/bin/bash

FFTLEN=4194304

#                1         2         3         4         5         6         7
#       123456789012345678901234567890123456789012345678901234567890123456789012
usage () 
{
    echo "                                                                        " >&2
    echo "NAME                                                                    " >&2
    echo "  sqtfp - creates time-frequency-power data file which can be made      " >&2
    echo "        - into a waterfall      " >&2
    echo "SYNOPSIS                                                                " >&2
    echo "  sqtfp [OPTIONS] files...                                              " >&2
    echo "OPTIONS                                                                 " >&2
    echo "  -l integer (optional), FFT length; default value is 4194304           " >&2
    echo "EXAMPLE                                                                 " >&2
    echo "  sqtfp -c 1420.0 2010-10-15-crab_1420_1-8bit-{01,02,03}.dat            " >&2
    echo "                                                                        " >&2
}

while getopts l: OPT
    do
        case $OPT in
        l) FFTLEN=$OPTARG;;
    esac
done

shift $(expr $OPTIND - 1)

FILES=$*
    
if [ ! "$FILES" ]; then
    usage
    exit 1
fi

# Process data to a time-frequency-power file
CMND="cat $FILES | sqsample -l $FFTLEN | sqwola -l $FFTLEN -f 9 -o 0 | sqfft -l $FFTLEN | sqpower -l $FFTLEN | sqreal -l $FFTLEN"

eval $CMND