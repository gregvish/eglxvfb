#!/bin/bash
set -e

NAME="AndriodEGLXvfb"
SDK=29

TOOLS="$ANDROID_HOME/build-tools/$SDK.*/"
PLATFORM="$ANDROID_HOME/platforms/android-$SDK/android.jar"

AAPT="$TOOLS/aapt"
DX="$TOOLS/dx"
ZIPALIGN="$TOOLS/zipalign"
APKSIGNER="$TOOLS/apksigner"

echo "Cleaning"
mkdir -p ./build
rm -rf ./build/*

echo "Compiling native activity"
mkdir -p ./build/lib/a
touch ./build/lib/a/k
echo "Compiling native glue"

echo "Generating R.java file"
$AAPT package -f -m -J ./build -M AndroidManifest.xml -S ./android_res -I $PLATFORM

echo "Compiling R.java file"
javac -d ./build -bootclasspath $PLATFORM -source 1.8 -target 1.8 ./build/com/*/R.java

echo "Translating in Dalvik bytecode"
$DX --min-sdk-version=$SDK --dex --output=./build/classes.dex ./build

echo "Making APK"
$AAPT package -f -m -F ./build/$NAME.unaligned.apk -M AndroidManifest.xml \
              -S ./android_res -I $PLATFORM
$AAPT add ./build/$NAME.unaligned.apk ./build/classes.dex

echo "Adding JNI to APK"
cd ./build
$AAPT add ./$NAME.unaligned.apk ./lib/*/*
cd -

echo "Aligning and signing APK"
$ZIPALIGN -f 4 ./build/$NAME.unaligned.apk ./build/$NAME.apk
$APKSIGNER sign --ks mykey.keystore -ks-pass pass:123456 ./build/$NAME.apk

