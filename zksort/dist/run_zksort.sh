#!/bin/bash
# Qt Creator Run 配置用脚本
PROJECT_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
export LD_LIBRARY_PATH="$PROJECT_ROOT/3rdparty/log4qt/lib/unix:$PROJECT_ROOT/3rdparty/qextserialport/lib/unix:$PROJECT_ROOT/3rdparty/jsoncpp/lib/unix:$PROJECT_ROOT/uikits/lib/unix"
cd "$(dirname "$0")"
exec ./zksort
