#!/bin/bash
set -e
trap 'kill $(jobs -p)' SIGINT SIGTERM EXIT

XVFB_DIR="/tmp/eglxvfb"
ABSTRACT_DIR="/eglxvfb"
XVFB_DISPLAY=":0"

BUILD_DIR=$(dirname "${BASH_SOURCE[0]}")/build

export XDG_RUNTIME_DIR=/run/user


mkdir -p $XVFB_DIR
Xvfb -screen 0 2160x1080x24+32 +render +extension GLX -noreset -nocursor \
     -fbdir $XVFB_DIR $XVFB_DISPLAY &
sleep 1
export DISPLAY=$XVFB_DISPLAY

$BUILD_DIR/fd_server $XVFB_DIR/Xvfb_screen0 $ABSTRACT_DIR/Xvfb_shm &


xrdb -merge $BUILD_DIR/../Xresources


export $(dbus-launch)
xfwm4 --sm-client-disable &

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/mesa/lib/aarch64-linux-gnu/
export VK_ICD_FILENAMES=/opt/mesa/share/vulkan/icd.d/freedreno_icd.aarch64.json
export GALLIUM_DRIVER=zink
export MESA_LOADER_DRIVER_OVERRIDE=zink

bash
