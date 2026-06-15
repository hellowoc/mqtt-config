#!/bin/sh

# version   : v1.0　
# date      : 20240826
# descrption: gpio44 sys status led
#             gpio45 zksort status led


if [ ! -d '/sys/class/gpio/gpio45' ];then
    echo 45 > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio45/direction
fi
sleep 2

while :
do
    zksortAlive=`ps |grep zksort|grep -v grep|wc -l`
    pmsAttachAlive=`ps |grep pmsAttach|grep -v grep|wc -l`
    if [ $zksortAlive = 1 ]; then
        echo 0 > /sys/class/gpio/gpio45/value
    else
        echo 1 > /sys/class/gpio/gpio45/value
    fi
    usleep $(($1*1000))
    if [ $zksortAlive = 1 ]; then
        echo 1 > /sys/class/gpio/gpio45/value
    fi
    if [ $zksortAlive = 0 ] && [ $pmsAttachAlive != 0 ]; then
	LOG=/sdcard/cnf/`date +%Y%m%d%H%M%S`_sys.log
	echo $LOG
        dmesg >$LOG
        exit 0
    fi
    usleep $(($1*1000))
done

