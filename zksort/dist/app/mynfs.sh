#!/bin/sh
ipcIpAddr=192.168.200.$2
localPath=/sdcard/$2
case "$1" in
    a)
        mount -t nfs -o intr,nolock,rsize=8192,wsize=8192 192.168.1.200:/3352 /mnt
        ;;
    b)
        mount -t nfs -o intr,nolock,rsize=8192,wsize=8192 162.254.129.200:/3352 /mnt
        ;;
    c)
        mount -t nfs -o nolock $ipcIpAddr:/home/linaro/ACS $localPath/ACS/
        mount -t nfs -o nolock $ipcIpAddr:/data            $localPath/data/
	    sleep 1
        ;;
    d)
        umount $localPath/ACS/
        umount $localPath/data/
        sleep 1
        ;;
    *)
        echo "Pls Use The Correct SW!"
esac


