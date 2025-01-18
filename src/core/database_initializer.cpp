#include "database_initializer.h"
#include <iostream>

/**
 * @brief Checks if the database is initialized by verifying the existence of the 'users' table.
 *
 * This function prepares and executes an SQL statement to check if a table named 'users' exists
 * in the database. It returns true if the table exists, indicating that the database is initialized,
 * and false otherwise.
 *
 * @param db A reference to the Database object to be checked.
 * @return true if the 'users' table exists in the database, false otherwise.
 */
bool DatabaseInitializer::isDatabaseInitialized(Database& db) {
    const char* sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='users';";
    sqlite3_stmt* stmt;
    bool initialized = false;

    if (sqlite3_prepare_v2(db.getDb(), sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            initialized = true;
        }
        sqlite3_finalize(stmt);
    }

    return initialized;
}

/**
 * @brief Initializes the database by creating necessary tables if they do not exist.
 *
 * This function creates the following tables:
 * - users: Stores user information with columns for id, username, and password.
 * - decks: Stores deck information with columns for id, user_id, and name. 
 *          It references the users table and deletes decks if the associated user is deleted.
 * - cards: Stores card information with columns for id, deck_id, question, answer, interval, 
 *          easeFactor, repetitions, lapses, and dueDate. It references the decks table and 
 *          deletes cards if the associated deck is deleted.
 *
 * @param db Reference to the Database object where the tables will be created.
 */
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
            interval INTEGER DEFAULT 1,
            easeFactor REAL DEFAULT 2.5,
            repetitions INTEGER DEFAULT 0,
            lapses INTEGER DEFAULT 0,
            dueDate INTEGER,
            FOREIGN KEY (deck_id) REFERENCES decks (id) ON DELETE CASCADE
        );
    )";

    db.executeSQL(createUsersTable, "Users table created.");
    db.executeSQL(createDecksTable, "Decks table created.");
    db.executeSQL(createCardsTable, "Cards table created.");
}