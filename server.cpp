#include "Server.h"
#include "EventLoop.h"
#include "Logger.h"
#include <Database.h>
#include <memory>

int terminate = 0;

int main() {

  std::string filename = "requests.db";
  
  Database& db = Database::getInstance(filename);

  Logger &logger = Logger::getInstance();

  logger.setLogFile(LOG_INFO, "info.log");
  logger.setLogLevel(LOG_INFO);

  logger.log(LOG_INFO, "EventLoop create~");
  std::shared_ptr<EventLoop> loop = std::make_shared<EventLoop>();

  logger.log(LOG_INFO, "Server create~");
  Server *server = Server::getServerInstance(loop);

  logger.log(LOG_INFO, "loop run ~~~");
  loop->loop();

  Server::destroyInstance();

  logger.destoryInstance();

  return 0;
}
