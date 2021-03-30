#!/bin/bash
ndk-build clean
ndk-build -C jni/
adb push images/ /data/local/tmp
adb push obj/local/arm64-v8a/libyuv.so /data/local/tmp
adb push obj/local/arm64-v8a/nv21_to_argb /data/local/tmp
width=640
height=360
adb shell "cd /data/local/tmp;export LD_LIBRARY_PATH=.;./nv21_to_argb ${width}x${height} images/${width}x${height}-nv21.yuv ./${width}x${height}.argb ./${width}x${height}.rgba"
adb pull /data/local/tmp/${width}x${height}.argb ./
adb pull /data/local/tmp/${width}x${height}.rgba ./
