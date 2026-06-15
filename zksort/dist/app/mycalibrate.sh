#!/bin/sh
count=0
while [ $count -lt 10 ]; do
    zksortAlive=`ps |grep ./zksort|grep -v grep|wc -l`
    if [ "$zksortAlive" -eq 0 ]; then
        break
    fi
    echo "zksortAlive: "$count >/tmp/zkalive
    usleep 100000
    count=$((count+1))
done                  

if [ $count -ge 10 ]; then
    /app/myrestart.sh
    exit 0           
fi 

LOWBOARD=`cat /proc/bus/input/devices |grep TSC2007|wc -l`

if [ ! -b /dev/mmcblk0p2 ]; then
    DRIVE="/mmcblk0p1"
else
    DRIVE="/mmcblk0p2"
fi

export PATH=$PATH:/opt/tslib/bin/
export LD_LIBRARY_PATH=/opt/tslib/lib:/opt/Qt4.8.4-AM335X/lib:/app/3rdparty:/usr/lib:$LD_LIBRARY_PATH

# set ts env
export TSLIB_ROOT=/opt/tslib
export TSLIB_TSDEVICE=/dev/input/touchscreen0
export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
export TSLIB_CALIBFILE=/media${DRIVE}/ts/pointercal
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export TSLIB_TSEVENTTYPE=input
export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts

# set Qt env
export QTDIR=/opt/Qt4.8.4-AM335X
export QWS_DISPLAY=:1
export QWS_KEYBOARD=TTY:/dev/tty1
export QWS_MOUSE_PROTO="Tslib MouseMan:/dev/input/mice"
export QT_PLUGIN_PATH=/opt/Qt4.8.4-AM335X/plugins


if [ $LOWBOARD = 0 ]; then
    /app/kmy.sh
    usleep 200000
    ./pmsAttach 9000 /dev/ttyO4 19200 &
    usleep 500000
    cat /proc/bus/input/devices >/sdcard/logs/.device
    event=$(cat /proc/bus/input/devices | sed -n '/PenMount Serial TouchScreen/,/ABS/p' | grep Handlers | tail -n1 | awk '{print $3}')
    rm -fr "$TSLIB_TSDEVICE"

    if [ -n "$event" ];then
        /bin/ln -sf "/dev/input/$event" $TSLIB_TSDEVICE
    else
        if [ ! -e /dev/input/event2 ]; then
            /bin/ln -sf /dev/input/event1 $TSLIB_TSDEVICE
        else
            /bin/ln -sf /dev/input/event2 $TSLIB_TSDEVICE
        fi
    fi
    usleep 100000    
fi 

/opt/tslib/bin/ts_calibrate
if [ ${DRIVE} = "/mmcblk0p1" ]; then 
    cp -fr /media${DRIVE}/ts/pointercal /media/mmcblk1p2/ts/pointercal
fi
sync

./zksort -qws -display VNC:LinuxFB &
