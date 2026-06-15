#!/bin/bash
cd /home/jie/桌面/v7.0.9/qtrun-aiuse/zksort/dist
export LD_LIBRARY_PATH=/home/jie/桌面/v7.0.9/qtrun-aiuse/3rdparty/log4qt/lib/unix:/home/jie/桌面/v7.0.9/qtrun-aiuse/3rdparty/qextserialport/lib/unix:/home/jie/桌面/v7.0.9/qtrun-aiuse/3rdparty/jsoncpp/lib/unix:/home/jie/桌面/v7.0.9/qtrun-aiuse/uikits/lib/unix:/home/jie/桌面/v7.0.9/qtrun-aiuse/3rdparty/mqtt
exec ./zksort
