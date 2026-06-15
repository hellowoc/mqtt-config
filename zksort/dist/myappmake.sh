#!/bin/sh
if [ -e app.tgz ];then
    rm app.tgz
    sync
fi
/usr/bin/rcc -binary ../resource/resource.qrc -o ./resource.rcc

cp zksort           -f  app/
cp resource.rcc     -f  app/
cp font             -fr app/fonts

cp ../resource/languages/*.ini  -f app/lang

if [ -e ../../3rdparty/jsoncpp/lib/unix/libjsoncpp.so.1.0.0 ]; then
    cp ../../3rdparty/jsoncpp/lib/unix/libjsoncpp.so.1.0.0 -f app/3rdparty/
fi
if [ -e ../../3rdparty/log4qt/lib/unix/liblog4qt.so.1.0.0 ]; then
    cp ../../3rdparty/log4qt/lib/unix/liblog4qt.so.1.0.0 -f app/3rdparty/
fi
if [ -e ../../3rdparty/qextserialport/lib/unix/libqextserialport.so.1.0.0 ]; then
    cp  ../../3rdparty/qextserialport/lib/unix/libqextserialport.so.1.0.0 -f app/3rdparty/
fi
if [ -e ../../uikits/lib/unix/libuikits.so.1.0.0 ];then
    cp ../../uikits/lib/unix/libuikits.so.1.0.0 -f app/3rdparty/
fi

chmod a+x app/zksort
chmod a+x app/*.sh

echo "`date`" >app/.log
sync
tar czf app.tgz app
sync
echo "app.tgz make ok"
exit 0

genext2fs -b 49152 -N 256 -d app app.bin
tune2fs -j -i 0 -c 0 app.bin

#rel="app_`date +%Y%m%d%H%M%S`.bin"
#mv app.bin $rel
#cp $rel /mnt/hgfs/myshare/
#sync
#
CUSTOM=$(gcc -E -dM ../global/custom.h | grep "^#define CUSTOM " | awk '{print $3}' | sed 's/"//g')
echo "\033[1;31m${CUSTOM}\033[0m app.bin make ok!"
