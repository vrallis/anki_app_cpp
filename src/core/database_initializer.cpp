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

    const char* createStudySessionsTable = R"(
        CREATE TABLE IF NOT EXISTS study_sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            deck_id INTEGER NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE,
            FOREIGN KEY (deck_id) REFERENCES decks (id) ON DELETE CASCADE
        );
    )";

    const char* createCardProgressTable = R"(
        CREATE TABLE IF NOT EXISTS card_progress (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            card_id INTEGER NOT NULL,
            due_date DATETIME NOT NULL,       -- Next review date
            interval INTEGER NOT NULL,        -- Current interval in days
            ease_factor REAL NOT NULL,        -- Factor affecting future intervals
            repetitions INTEGER NOT NULL,     -- Number of reviews
            lapses INTEGER NOT NULL,          -- Number of lapses (incorrect answers)
            FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE,
            FOREIGN KEY (card_id) REFERENCES cards (id) ON DELETE CASCADE
        );
    )";

    // Execute the SQL statements to create tables
    db.executeSQL(createUsersTable, "Table 'users' created.");
    db.executeSQL(createDecksTable, "Table 'decks' created.");
    db.executeSQL(createCardsTable, "Table 'cards' created.");
    db.executeSQL(createStudySessionsTable, "Table 'study_sessions' created.");
    db.executeSQL(createCardProgressTable, "Table 'card_progress' created.");
}

bool DatabaseInitializer::isDatabaseInitialized(Database& db) {
    const char* sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";
    sqlite3_stmt* stmt;

    bool initialized = false;

    if (sqlite3_prepare_v2(db.db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // If the query returns a row, the 'users' table exists
        initialized = (sqlite3_step(stmt) == SQLITE_ROW);
    } else {
        std::cerr << "Failed to prepare statement for database initialization check: "
                  << sqlite3_errmsg(db.db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return initialized;
}

