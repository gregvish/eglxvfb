#!/bin/bash
set -e

mkdir -p ./build
rm -rf ./build/*

gcc -Werror -Wall -pedantic -o ./build/Xtest xtest.c \
    -lX11 -lXtst

gcc -Werror -Wall -pedantic -o ./build/Xdamage damage.c \
    -lX11 -lXdamage -lXfixes -lpthread

gcc -Werror -Wall -pedantic -o ./build/XEGLXvfb eglxvfb.c x_eglxvfb.c \
    -lGLESv2 -lEGL -lX11 -lpthread

gcc -Werror -Wall -pedantic -shared -fPIC -o ./build/shm_shim.so shm_shim.c

