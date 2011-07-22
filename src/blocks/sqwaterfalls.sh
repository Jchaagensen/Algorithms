#!/bin/bash

BW=8.738133333
CHANNLS=4096

#                1         2         3         4         5         6         7
#       123456789012345678901234567890123456789012345678901234567890123456789012
usage () {
  echo "                                                                        " >&2
  echo "NAME                                                                    " >&2
  echo "  sqwaterfalls - create waterfall images for setiQuest observation      " >&2
  echo "SYNOPSIS                                                                " >&2
  echo "  sqwaterfalls [OPTIONS] file...                                        " >&2
  echo "OPTIONS                                                                 " >&2
  echo "  -c real (required), observation center frequency (MHz)                " >&2
  echo "  -b real, bandwidth (MHz) - default value is 8.738 MHz                 " >&2
  echo "  -n integer, number of channels - default value is 4096                " >&2
  echo "  -o output directory name                                              " >&2
  echo "EXAMPLE                                                                 " >&2
  echo "  sqwaterfalls -c 1420.0 time-frequency-power.dat                       " >&2
  echo "                                                                        " >&2
}

while getopts c:b:n:o: OPT
do
  case $OPT in
    c) CFREQ=$OPTARG;;
    b) BW=$OPTARG;;
    n) CHANNLS=$OPTARG;;
    o) IMAGEDIR=$OPTARG;;
  esac
done

shift $(expr $OPTIND - 1)

FILE=$*
    
if [ ! "$FILE" ]; then
    usage
    exit 1
fi

if [ ! "$CFREQ" ] || [ ! "$IMAGEDIR" ]; then
  usage
  exit 1
fi

if [ ! -d $IMAGEDIR ]; then
  mkdir $IMAGEDIR
fi

for CHAN in $(seq -1536 1 1535)
do
  for OFST in $(seq 0 1 7)
  do
    SUBDIR=$(printf "chan%+05d" $CHAN)
    if [ ! -d $IMAGEDIR/$SUBDIR ]; then
      mkdir $IMAGEDIR/$SUBDIR
    fi
    printf -v LFREQ "%.6f" $(echo "scale=10; $CFREQ + (($BW/$CHANNLS)*($CHAN+(($OFST-4)/8)));" | bc) 
    printf -v RFREQ "%.6f" $(echo "scale=10; $CFREQ + (($BW/$CHANNLS)*($CHAN+(($OFST-3)/8)));" | bc) 
    CMND="sqgetimgtfp -c $CHAN -o $OFST $FILE | sqpnm -c 256 -r 340 -x | convert - -depth 16 -type GrayScale -flip -gamma 1.2 -depth 8 -quality 90 $IMAGEDIR/${SUBDIR}/${SUBDIR}-${OFST}-${LFREQ}-${RFREQ}.png"
    echo $CMND >&2
    eval $CMND
  done
done

