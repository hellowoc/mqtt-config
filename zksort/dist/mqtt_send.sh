#!/bin/bash
if [ -z "$1" ]; then
    echo "用法: ./mqtt_send.sh '命令内容'"
    echo "示例: ./mqtt_send.sh '{\"cmd\":[{\"cmdCode\":\"CMD_Feed\",\"content\":\"50:20\",\"uuid\":\"test\"}]}'"
    exit 1
fi
mosquitto_pub -t "/ZKGDDEV47" -h 127.0.0.1 -m "$1" && echo "已发送 → /ZKGDDEV47"
