#include "database.h"
#include <iostream>
#include <sqlite3.h>

Database::Database(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Database opened successfully!" << std::endl;
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::addUser(const User& user) {
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "User '" << user.getUsername() << "' created successfully!" << std::endl;
    }
    sqlite3_finalize(stmt);
}

bool Database::verifyUser(const std::string& username, const std::string& password) {
    const char* sql = "SELECT id FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return success;
}

void Database::createDeck(int userId, const std::string& deckName) {
    const char* sql = "INSERT INTO decks (user_id, name) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, deckName.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error creating deck: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Deck '" << deckName << "' created successfully!" << std::endl;
    }
    sqlite3_finalize(stmt);
}

void Database::listDecks(int userId) {
    const char* sql = "SELECT id, name FROM decks WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);

    std::cout << "Decks for user " << userId << ":" << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int deckId = sqlite3_column_int(stmt, 0);
        const char* deckName = (const char*)sqlite3_column_text(stmt, 1);
        std::cout << "  Deck ID: " << deckId << ", Name: " << deckName << std::endl;
    }
    sqlite3_finalize(stmt);
}

bool Database::hasUsers() {
    const char* sql = "SELECT COUNT(*) FROM users;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    bool hasUsers = sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0;
    sqlite3_finalize(stmt);
    return hasUsers;
}

void Database::executeSQL(const char* sql, const std::string& successMessage) {
    char* errMsg = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error executing SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << successMessage << std::endl;
    }
}

