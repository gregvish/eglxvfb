#!/bin/bash
set -e
trap 'kill $(jobs -p)' SIGINT SIGTERM EXIT

XVFB_DIR="/tmp/xeglxvfb"
XVFB_DISPLAY=":1"

mkdir -p $XVFB_DIR
cd ./build

Xvfb -screen 0 1920x1080x24+32 +extension GLX +render -noreset -nocursor \
     -fbdir $XVFB_DIR $XVFB_DISPLAY &
sleep 1

DISPLAY=$XVFB_DISPLAY xrdb -merge ../Xresources
DISPLAY=$XVFB_DISPLAY socat unix-l:$XVFB_DIR/Xdamage,fork exec:./Xdamage &
DISPLAY=$XVFB_DISPLAY socat unix-l:$XVFB_DIR/Xtest,fork exec:./Xtest &
sleep 1

DISPLAY=$XVFB_DISPLAY TERMINAL=kitty i3 &
DISPLAY=$XVFB_DISPLAY kitty &
DISPLAY=:0 ./XEGLXvfb $XVFB_DIR

