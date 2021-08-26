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

DISPLAY=$XVFB_DISPLAY socat unix-l:$XVFB_DIR/Xdamage,fork exec:./Xdamage &
DISPLAY=$XVFB_DISPLAY socat unix-l:$XVFB_DIR/Xtest,fork exec:./Xtest &
DISPLAY=$XVFB_DISPLAY icewm &
sleep 1
DISPLAY=$XVFB_DISPLAY vglrun -v -d :0 glxgears -info &

DISPLAY=:0 ./XEGLXvfb $XVFB_DIR

