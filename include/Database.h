#include <ctime>
#include <iostream>
#include <sqlite3.h>
#include <string>

class Database {
private:
  sqlite3 *db;

  Database(const std::string &filename) {
    int rc = sqlite3_open(filename.c_str(), &db);
    if (rc != SQLITE_OK) {
      std::cerr << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
      throw std::runtime_error("数据库打开失败");
    }

    createTable();
  }

public:
  static Database &getInstance(const std::string &filename) {
    static Database instance(filename);
    return instance;
  }

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  ~Database() { sqlite3_close(db); }

  void createTable() {
    std::string sql = "CREATE TABLE IF NOT EXISTS requests ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "method TEXT NOT NULL,"
                      "ip TEXT NOT NULL,"
                      "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                      ")";

    char *errMsg;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
      std::cerr << "创建表失败: " << errMsg << std::endl;
      sqlite3_free(errMsg);
      throw std::runtime_error("创建表失败");
    }
  }

  void insertRequest(const std::string &method, const std::string &ip) {
    std::string sql = "INSERT INTO requests (method, ip) VALUES (?, ?)";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
      std::cerr << "预处理失败: " << sqlite3_errmsg(db) << std::endl;
      throw std::runtime_error("预处理失败");
    }

    rc = sqlite3_bind_text(stmt, 1, method.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
      std::cerr << "绑定参数失败: " << sqlite3_errmsg(db) << std::endl;
      throw std::runtime_error("绑定参数失败");
    }

    rc = sqlite3_bind_text(stmt, 2, ip.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
      std::cerr << "绑定参数失败: " << sqlite3_errmsg(db) << std::endl;
      throw std::runtime_error("绑定参数失败");
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      std::cerr << "执行插入失败: " << sqlite3_errmsg(db) << std::endl;
      throw std::runtime_error("执行插入失败");
    }

    sqlite3_finalize(stmt);
  }
};
