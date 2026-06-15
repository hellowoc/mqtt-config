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

/app/kmy.sh           
usleep 500000
/app/mystart.sh
exit 0           
