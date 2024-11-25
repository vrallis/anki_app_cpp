#include "database_initializer.h"
#include <iostream>

void DatabaseInitializer::initialize(Database& db) {
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
            FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE
        );
    )";

    const char* createCardsTable = R"(
        CREATE TABLE IF NOT EXISTS cards (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            deck_id INTEGER NOT NULL,
            question TEXT NOT NULL,
            answer TEXT NOT NULL,
            FOREIGN KEY (deck_id) REFERENCES decks (id) ON DELETE CASCADE
        );
    )";

    db.executeSQL(createUsersTable, "Table 'users' created.");
    db.executeSQL(createDecksTable, "Table 'decks' created.");
    db.executeSQL(createCardsTable, "Table 'cards' created.");
}

bool DatabaseInitializer::isDatabaseInitialized(Database& db) {
    const char* sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db.db, sql, -1, &stmt, nullptr);
    bool initialized = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return initialized;
}
