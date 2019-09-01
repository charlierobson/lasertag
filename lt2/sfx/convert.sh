#!/bin/bash

function compile {
  ffmpeg -nostats -loglevel 0 -y -i $1.mp3 -ar 22050 -ac 1 -f u8 -acodec pcm_u8 $1.raw

  echo -n -e "const " >sfx_$1.h
  xxd -i $1.raw >>sfx_$1.h
  rm -f $1.raw
}

rm -f *.h
rm -f *.raw

compile hello
compile shot
compile empty
compile reload
compile beep
compile shuttingdown
