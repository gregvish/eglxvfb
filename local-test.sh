#!/bin/bash
set -e
trap 'kill $(jobs -p)' SIGINT SIGTERM EXIT

XVFB_DIR="/tmp/eglxvfb"
ABSTRACT_DIR="/eglxvfb"
XVFB_DISPLAY=":1"

BUILD_DIR=$(dirname "${BASH_SOURCE[0]}")/build

mkdir -p $XVFB_DIR

Xvfb -screen 0 1920x1080x24+32 +extension GLX +render -noreset -nocursor \
     -fbdir $XVFB_DIR $XVFB_DISPLAY &
sleep 1

DISPLAY=$XVFB_DISPLAY $BUILD_DIR/fd_server $XVFB_DIR/Xvfb_screen0 $ABSTRACT_DIR/Xvfb_shm &

DISPLAY=$XVFB_DISPLAY xrdb -merge $BUILD_DIR/../Xresources
DISPLAY=$XVFB_DISPLAY TERMINAL=kitty xfwm4 &
DISPLAY=$XVFB_DISPLAY TERMINAL=kitty xfce4-panel &

sleep 1
DISPLAY=:0 $BUILD_DIR/XEGLXvfb $ABSTRACT_DIR

