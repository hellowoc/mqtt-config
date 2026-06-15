#!/bin/sh
export LD_LIBRARY_PATH=/opt/Qt5.15/ext/lib:/home/app/3rdparty:/usr/lib:$LD_LIBRARY_PATH

ln -sf /dev/ttyS5 /dev/ttySAC1
ln -sf /dev/ttyS3 /dev/ttySAC2
ln -sf /dev/ttyS7 /dev/ttySAC3

mkdir /dev/i2c
ls -s /dev/i2c-2 /dev/i2c/0


# set ip
ifconfig eth0 192.168.1.100 netmask 255.255.255.0 up
ifconfig eth1 162.254.129.100 netmask 255.255.255.0 up


mv  /sdcard/cnf/.log /sdcard/cnf/.log_last
sync
#nohup ./zksort -qws -display VNC:LinuxFB >/sdcard/cnf/.log 2>&1 &
./zksort &
exit 0

