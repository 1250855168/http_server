#!/bin/bash

# 函数：检查服务器指定端口的连通性
is_server_port_reachable() {
  # TODO: 替换为你的服务器IP和端口号
  server_ip="192.168.157.128"
  server_port="1234"

#> /dev/null 表示将命令的标准输出重定向到 /dev/null 设备文件中。
#/dev/null 是一个特殊的设备文件，它会丢弃一切写入其中的数据。因此，
#使用 > /dev/null 可以将命令的输出丢弃，不在终端上显示。
#2>&1 表示将命令的标准错误输出重定向到与标准输出相同的位置。
#2 表示标准错误输出的文件描述符，&1 表示指向标准输出的文件描述符。
#因此，使用 2>&1 可以将命令的错误输出与标准输出合并，一同重定向到同一位置（即 /dev/null

  # 使用nc命令检查端口连通性，超时时间设置为2秒
  nc -z -w 2 "$server_ip" "$server_port" > /dev/null 2>&1
  if [ $? -eq 0 ]; then
    return 1
  else
    return 0
  fi
}

# 函数：重启服务器
restart_server() {
  # TODO: 实现重启服务器的逻辑
  # 示例：通过systemctl命令重启服务器服务
    echo "Server restarted."
   /home/china/project/http_server/build/server  
}

# 函数：清空日志文件
clear_log_file() {
  # TODO: 实现清空日志文件的逻辑
  # 示例：通过重定向将空内容写入日志文件
  > /home/china/project/http_server/build/info.log
}

# 循环执行检查和操作
while true; do
  # 第一次检查
  if is_server_port_reachable; then
    echo "Server port is reachable."
  else
    echo "Server port is not reachable. Restarting..."
    restart_server
  fi

  # 第二次检查
  if is_server_port_reachable; then
    echo "Server port is reachable."
  else
    echo "Server port is not reachable. Restarting..."
    restart_server
  fi

  # 获取当前时间的星期几
  day_of_week=$(date +%u)

  # 每周一清空日志文件（星期一对应的数字为1）
  if [ "$day_of_week" -eq 1 ]; then
    echo "Clearing log file..."
    clear_log_file
  fi

  # 等待1小时
  sleep 1h
done