#include "database.h"
#include <iostream>
#include <filesystem>

Database::Database(const std::string& db_name) {
    bool dbExists = std::filesystem::exists(db_name);

    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Database opened successfully!" << std::endl;
        if (!dbExists) {
            initialize();
        }
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::initialize() {
    std::cout << "Initializing database..." << std::endl;

    const char* createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );
    )";

    const char* createDecksTable = R"(
        CREATE TABLE IF NOT EXISTS decks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users (id)
        );
    )";

    const char* createCardsTable = R"(
        CREATE TABLE IF NOT EXISTS cards (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            deck_id INTEGER NOT NULL,
            question TEXT NOT NULL,
            answer TEXT NOT NULL,
            FOREIGN KEY (deck_id) REFERENCES decks (id)
        );
    )";

    executeSQL(createUsersTable, "Table 'users' created.");
    executeSQL(createDecksTable, "Table 'decks' created.");
    executeSQL(createCardsTable, "Table 'cards' created.");
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

bool Database::hasUsers() {
    const char* sql = "SELECT COUNT(*) FROM users;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int userCount = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return userCount > 0;
    }
    sqlite3_finalize(stmt);
    return false;
}

void Database::addFirstUser() {
    if (!hasUsers()) {
        std::cout << "No users found. Please create first user!" << std::endl;

        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        User user(username, password);
        addUser(user);

        std::cout << "User '" << username << "' created successfully!" << std::endl;
    }
}

void Database::addUser(const User& user) {
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}
