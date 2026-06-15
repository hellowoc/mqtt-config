#!/bin/sh

sleep 2

count1=`ps |grep zksort|grep -v grep|wc -l`

if [ $count1 = 0 ]; then
    sleep 3
    count1=`ps |grep zksort|grep -v grep|wc -l`
    count2=`ps |grep pmsAttach|grep -v grep|wc -l`
    count3=`ps |grep mycalibrate|grep -v grep|wc -l`
    count4=`ps |grep Myupdate|grep -v grep|wc -l`

    ## cnf not match
    if [ $count1 = 0 ] && [ $count2 != 0 ] && [ $count3 = 0 ] && [ $count4 = 0 ]; then
        cd /app/
        ./kmy.sh
        ./MyUpdate.sh 
    fi
fi

