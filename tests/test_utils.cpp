#include "test_utils.h"
#include "core/database.h"
#include <cstdio> // For std::remove

void setupTestDatabase() {
    std::remove("test.db"); // Delete the test database if it exists
    Database db("test.db");
    const char* sql = "CREATE TABLE users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT UNIQUE,"
                      "password TEXT);";
    sqlite3_exec(db.getDb(), sql, nullptr, nullptr, nullptr);
}