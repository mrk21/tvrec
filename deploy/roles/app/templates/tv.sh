#!/bin/sh
ch=$1
if [ -z $ch ]; then
  echo "Invalid channel!!"
  exit 1
fi
#recpt1 --b25 --strip --sid hd $ch - - | cvlc -f -
recpt1 --b25 $ch - - | cvlc -f -

