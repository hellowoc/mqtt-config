#!/bin/bash
usleep 10000

ipa=`echo $1|cut -d. -f1`
ipb=`echo $1|cut -d. -f2`
start_h=`echo $1|cut -d. -f3`
start_l=`echo $1|cut -d. -f4`

end_h=`echo $2|cut -d. -f3`
end_l=`echo $2|cut -d. -f4`
tmp="/tmp/pingres"
res="/sdcard/ts/active_ip.txt"

echo $ipa.$ipb.$start_h.$start_l - $ipa.$ipb.$end_h.$end_l
if [ -e  $res ];then
	rm $res
fi
touch $res

for ((i=start_h;i<=end_h;i++))
do
    for ((j=start_l;j<=end_l;j++))
    do
        echo null > $tmp
    ping -s 32 -c 2 -w 1 $ipa.$ipb.$i.$j > $tmp
    cnt=`cat $tmp|grep ttl|wc -l`
	if [ $cnt -gt 0 ]; then
        echo "(√) $ipa.$ipb.$i.$j">> $res
	else
        echo "(x) $ipa.$ipb.$i.$j">> $res
	fi
    done
done
