#!/bin/bash
set -e

NAME="AndriodEGLXvfb"
SDK=26
NDK=22
LIB_ARCH="arm64-v8a"
TARGET="aarch64-linux-android"


TOOLS="$ANDROID_HOME/build-tools/$SDK.*/"
PLATFORM="$ANDROID_HOME/platforms/android-$SDK/android.jar"

AAPT="$TOOLS/aapt"
DX="$TOOLS/dx"
ZIPALIGN="$TOOLS/zipalign"
APKSIGNER="$TOOLS/apksigner"


NDK_PATH="$ANDROID_HOME/ndk/$NDK.*/"
TOOLCHAIN="$NDK_PATH/toolchains/llvm/prebuilt/linux-*/bin/"
PREFIX="$TOOLCHAIN/$TARGET"
CC="$PREFIX$SDK-clang"

GLUE="$NDK_PATH/sources/android/native_app_glue/"
GLUE_CODE="$GLUE/android_native_app_glue.c"


echo "Cleaning"
rm -rf ./build
mkdir -p ./build
mkdir -p ./build/lib/$LIB_ARCH
mkdir -p ./build/bin


echo "Compiling native app glue"
$CC -Werror -Wall -c $GLUE_CODE -o ./build/native_app_glue.o
$PREFIX-ar rcs ./build/libnative_app_glue.a ./build/native_app_glue.o

echo "Compiling our native activity"
$CC -Werror -Wall -pedantic -std=gnu11 -fPIC -shared \
    -I $GLUE -L ./build \
    -DANDROID_BUILD \
    eglxvfb.c \
    android_eglxvfb.c \
    -o ./build/lib/$LIB_ARCH/libandroideglxvfb.so \
    -u ANativeActivity_onCreate \
    -llog -landroid -lEGL -lGLESv2 -lnative_app_glue


echo "Generating R.java file"
$AAPT package -f -m -J ./build -M AndroidManifest.xml -S ./android_res -I $PLATFORM

echo "Compiling R.java file"
javac -d ./build -bootclasspath $PLATFORM -source 1.8 -target 1.8 ./build/com/*/R.java

echo "Translating in Dalvik bytecode"
$DX --min-sdk-version=$SDK --dex --output=./build/bin/classes.dex ./build

echo "Making APK, adding native lib"
$AAPT package -f -m -F ./build/$NAME.unaligned.apk -M AndroidManifest.xml \
              -S ./android_res -I $PLATFORM
cd ./build
$AAPT add ./$NAME.unaligned.apk bin/classes.dex
$AAPT add ./$NAME.unaligned.apk lib/*/*
cd -

echo "Aligning and signing APK"
$ZIPALIGN -f 4 ./build/$NAME.unaligned.apk ./build/$NAME.apk
$APKSIGNER sign --ks mykey.keystore -ks-pass pass:123456 ./build/$NAME.apk

