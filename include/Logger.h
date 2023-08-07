#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <mutex>
#include <ctime>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogMessage {
    LogLevel level;
    std::string message;
    std::string timestamp;
};

class Logger {
public:
    static Logger& getInstance();

    void setLogFile(LogLevel level, const std::string& filename);
    void setLogLevel(LogLevel level);

    void log(LogLevel level, const std::string& message);
    void flush();

private:
    Logger();
    ~Logger();

    void writeToFile(const LogMessage& logMsg);
    void writeToFileBatch();

    std::string getCurrentTimestamp();

    std::string getLogLevelString(LogLevel level);

    std::mutex queueMutex;
    std::vector<LogMessage> pendingLogs;

    std::ofstream logFiles[4];
    LogLevel logLevel;
};

#endif  // LOGGER_H