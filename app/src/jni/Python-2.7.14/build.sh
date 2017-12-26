TOOLPATH=/home/dongjunjie/Android/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
LOCAL=`pwd`
COMMON_CFLAGS=-g -target armv7-none-linux-androideabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -Os -DNDEBUG \
        -I/home/dongjunjie/Android/android-ndk-r14b/sources/android/support/include -D__ANDROID__ -DANDROID \
        -I/home/dongjunjie/Android/android-ndk-r14b/platforms/android-16/arch-arm/usr/include \
        --sysroot=/home/dongjunjie/Android/android-ndk-r14b/platforms/android-16/arch-arm

COMMON_LDFLAGS=-L/home/dongjunjie/Android/android-ndk-r14b/platforms/android-16/arch-arm/usr/lib

CC=${TOOLPATH}/bin/arm-linux-androideabi-gcc \
CXX=${TOOLPATH}/bin/arm-linux-android-g++ \
AR=${TOOLPATH}/bin/arm-linux-android-ar \
LD=${TOOLPATH}/bin/arm-linux-android-ld \
CFLAGS="$COMMON_CFLAGS" \
LDFLAGS="$COMMON_LDFLAGS" \
./configure --prefix=${LOCAL}/out
