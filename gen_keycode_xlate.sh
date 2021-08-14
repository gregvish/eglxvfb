#!/bin/bash
set -e

gcc -o ./build/keycode_to_keysym keycode_to_keysym.c -lX11
./build/keycode_to_keysym
