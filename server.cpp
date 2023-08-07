#include "Server.h"
#include "EventLoop.h"
#include "Logger.h"

int terminate = 0;

int main() {

  Logger &logger = Logger::getInstance();

  logger.setLogFile(LOG_INFO, "info.log");
  logger.setLogLevel(LOG_INFO);

  logger.log(LOG_INFO, "EventLoop create~");
  EventLoop *loop = new EventLoop();

  logger.log(LOG_INFO, "Server create~");
  Server *server = Server::getInstance(loop);

  logger.log(LOG_INFO, "loop run ~~~");
  loop->loop();

  Server::destoryInstance();

  logger.destoryInstance();

  return 0;
}
