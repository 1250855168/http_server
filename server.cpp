#include "Server.h"
#include "EventLoop.h"
#include "Logger.h"

int terminate = 0;

int main() {

  Logger &logger = Logger::getInstance();

  logger.setLogFile(LOG_INFO, "info.log");
  logger.setLogLevel(LOG_INFO);

  for (int i = 0; i < 1000; ++i) {
    logger.log(LOG_INFO, "This is an info message");
  }

  logger.flush();

  EventLoop *loop = new EventLoop();

  Server *server = Server::getInstance(loop);

  loop->loop();

  Server::destoryInstance();

  return 0;
}
