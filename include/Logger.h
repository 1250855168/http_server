#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

enum LogLevel { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR };

struct LogMessage {
  LogLevel level;       // 日志级别
  std::string message;  // 日志消息
  std::string timestamp;  // 时间戳
};

class Logger {
public:
  static Logger &getInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance == nullptr) {
      instance = new Logger();
    }
    return *instance;
  }

  void setLogFile(LogLevel level, const std::string &filename);  // 设置日志文件名
  void setLogLevel(LogLevel level);  // 设置日志级别

  void log(LogLevel level, const std::string &message);  // 记录日志
  void flush();  // 刷新日志缓冲区
  void destoryInstance() {
    for (auto &file : logFiles) {
      if (file.is_open()) {
        file.close();
      }
    }
  }  // 销毁 Logger 实例时关闭所有日志文件

private:
  Logger();
  ~Logger();
  
  static Logger *instance;  // Logger 单例实例指针
  static std::mutex mtx;    // 互斥锁

  void writeToFile(const LogMessage &logMsg);  // 将日志消息写入文件
  void writeToFileBatch();  // 批量将待写入的日志消息写入文件

  std::string getCurrentTimestamp();  // 获取当前时间戳

  std::string getLogLevelString(LogLevel level);  // 获取日志级别的字符串表示

  std::mutex queueMutex;  // 待写入日志消息队列的互斥锁
  std::vector<LogMessage> pendingLogs;  // 待写入的日志消息队列

  std::ofstream logFiles[4];  // 日志文件输出流数组，包括 DEBUG、INFO、WARNING 和 ERROR 四个级别
  LogLevel logLevel;  // 当前日志级别
};

#endif // LOGGER_H