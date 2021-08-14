#!/bin/bash
set -e

gcc -o keycode_to_keysym keycode_to_keysym.c -lX11
./keycode_to_keysym
