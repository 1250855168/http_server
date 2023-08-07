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
  LogLevel level;
  std::string message;
  std::string timestamp;
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

  void setLogFile(LogLevel level, const std::string &filename);
  void setLogLevel(LogLevel level);

  void log(LogLevel level, const std::string &message);
  void flush();
  void destoryInstance() {
    for (auto &file : logFiles) {
      if (file.is_open()) {
        file.close();
      }
    }
  }

private:
  Logger();
  ~Logger();
  
  static Logger *instance;
  static std::mutex mtx;

  void writeToFile(const LogMessage &logMsg);
  void writeToFileBatch();

  std::string getCurrentTimestamp();

  std::string getLogLevelString(LogLevel level);

  std::mutex queueMutex;
  std::vector<LogMessage> pendingLogs;

  std::ofstream logFiles[4];
  LogLevel logLevel;
};

#endif // LOGGER_H