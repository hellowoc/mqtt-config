#!/bin/bash
echo "╔══════════════════════════════════════════╗"
echo "║     MQTT 端到端通信可视化监控            ║"
echo "║     Topic: /ZKGDDEV47                    ║"
echo "║     Broker: 127.0.0.1:1883               ║"
echo "╚══════════════════════════════════════════╝"
echo ""
echo "上方窗口 = zksort 内部 MQTT 日志"
echo "下方窗口 = Broker 转发的原始消息"
echo "──────────────────────────────────────────"
echo "  按 Ctrl+C 退出"
echo ""

# 确保 Broker 在运行
if ! systemctl is-active --quiet mosquitto; then
    echo "正在启动 Broker..."
    echo "123" | sudo -S systemctl start mosquitto 2>/dev/null
    sleep 1
fi

# 上方: zksort 日志（MQTT 线程视角）
tail -f /tmp/zksort.log 2>/dev/null | grep --line-buffered -iE 'MQTT|connect|订阅|收到|subscribe|message|CMD|recv|来自' &
LOG_PID=$!

# 下方: Broker 消息（Broker 视角）
mosquitto_sub -t "/ZKGDDEV47" -h 127.0.0.1 -v 2>/dev/null | while read -r line; do
    topic=$(echo "$line" | cut -d' ' -f1)
    payload=$(echo "$line" | cut -d' ' -f2-)
    echo ""
    echo "══════════════════════════════════════"
    echo "  $(date '+%H:%M:%S') Broker 收到消息 → 转发给 zksort"
    echo "──────────────────────────────────────"
    echo "  $(echo "$payload" | python3 -m json.tool 2>/dev/null || echo "$payload")"
    echo "══════════════════════════════════════"
done

kill $LOG_PID 2>/dev/null
