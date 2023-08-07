#include "Logger.h"

Logger::Logger() : logLevel(LOG_DEBUG) {}

Logger::~Logger() {}

Logger* Logger::instance = nullptr;
std::mutex Logger::mtx;

void Logger::setLogFile(LogLevel level, const std::string &filename) {
  int index = static_cast<int>(level);
  if (index < 0 || index >= 4) {
    throw std::runtime_error("Invalid log level!");
  }

  logFiles[index].open(filename, std::ios::app);
  if (!logFiles[index].is_open()) {
    throw std::runtime_error("Unable to open log file: " + filename);
  }
}

void Logger::setLogLevel(LogLevel level) { logLevel = level; }

void Logger::log(LogLevel level, const std::string &message) {
  if (level < logLevel) {
    return;
  }

  LogMessage logMsg;
  logMsg.level = level;
  logMsg.message = message;
  logMsg.timestamp = getCurrentTimestamp();

  std::lock_guard<std::mutex> lock(queueMutex);
  pendingLogs.push_back(logMsg);

  if (pendingLogs.size() >= 10) {
    writeToFileBatch();
  }
}

void Logger::flush() {
  std::lock_guard<std::mutex> lock(queueMutex);
  writeToFileBatch();
}

void Logger::writeToFile(const LogMessage &logMsg) {
  int index = static_cast<int>(logMsg.level);
  if (index < 0 || index >= 4 || !logFiles[index].is_open()) {
    throw std::runtime_error("Invalid log level or log file is not open!");
  }

  logFiles[index] << logMsg.timestamp << " - "
                  << getLogLevelString(logMsg.level) << ": " << logMsg.message
                  << std::endl;
}

void Logger::writeToFileBatch() {
  if (pendingLogs.empty()) {
    return;
  }

  for (const LogMessage &logMsg : pendingLogs) {
    writeToFile(logMsg);
  }

  pendingLogs.clear();
}

std::string Logger::getCurrentTimestamp() {
  std::time_t now = std::time(nullptr);
  std::tm *timeinfo = std::localtime(&now);
  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return std::string(buffer);
}

std::string Logger::getLogLevelString(LogLevel level) {
  switch (level) {
  case LOG_DEBUG:
    return "DEBUG";
  case LOG_INFO:
    return "INFO";
  case LOG_WARNING:
    return "WARNING";
  case LOG_ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}