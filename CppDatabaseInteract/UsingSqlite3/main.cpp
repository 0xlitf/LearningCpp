#include "sqlite3.h"
#include <iostream>

// #pragma comment(lib, "sqlite3.lib")

int main() {
    // 打开或创建数据库文件
    sqlite3* db;
    const char* path = "db.sqlite3";
    int rc = sqlite3_open(path, &db);
    if (rc == SQLITE_OK) {
        std::cout << "open success" << std::endl;
    } else {
        std::cerr << "open failed: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3_close(db);

    return 0;
}
