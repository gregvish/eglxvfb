#!/bin/bash
set -e

mkdir -p ./build
rm -rf ./build/*

gcc -Werror -Wall -pedantic -o ./build/XEGLXvfb eglxvfb.c x_eglxvfb.c \
    -lGLESv2 -lEGL -lX11 -lpthread

gcc -Werror -Wall -pedantic -o ./build/fd_server fd_server.c xtest.c xdamage.c \
    -lX11 -lXtst -lXdamage -lXfixes -lpthread

gcc -Werror -Wall -pedantic -shared -fPIC -o ./build/shm_shim.so shm_shim.c

