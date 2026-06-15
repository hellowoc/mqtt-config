#!/bin/bash
# 色选机环境初始化脚本 — 每次开机后执行一次

echo "=== 创建 /sdcard 目录 ==="
echo "123" | sudo -S mkdir -p /sdcard/logs /sdcard/cnf /sdcard/ts /sdcard/logo 2>/dev/null
echo "123" | sudo -S chmod -R 777 /sdcard 2>/dev/null

echo "=== 创建虚拟串口 ==="
# 先清理旧串口
echo "123" | sudo -S rm -f /dev/ttySAC1 /dev/ttySAC2 /dev/ttySAC1_out /dev/ttySAC2_out 2>/dev/null
# 杀掉旧 socat
pkill -f "socat.*ttySAC" 2>/dev/null
sleep 0.5
# 创建虚拟串口对
echo "123" | sudo -S socat PTY,link=/dev/ttySAC1,raw,echo=0,mode=666 PTY,link=/dev/ttySAC1_out,raw,echo=0,mode=666 &
sleep 0.5
echo "123" | sudo -S socat PTY,link=/dev/ttySAC2,raw,echo=0,mode=666 PTY,link=/dev/ttySAC2_out,raw,echo=0,mode=666 &
sleep 0.5
ls -la /dev/ttySAC*

echo "=== 环境初始化完成 ==="
