#include "database.h"
#include <iostream>
#include <sqlite3.h>
#include "database_initializer.h"

Database::Database(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        if (!DatabaseInitializer::isDatabaseInitialized(*this)) {
            std::cout << "Database not detected. Creating one right now!" << std::endl;
            DatabaseInitializer::initialize(*this);
        }
        std::cout << "Database opened successfully!" << std::endl;
    }
}


Database::~Database() {
    sqlite3_close(db);
}

void Database::addUser(const User& user) {
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "User '" << user.getUsername() << "' added to the database.\n";
        } else {
            std::cerr << "Error adding user: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare addUser query: " << sqlite3_errmsg(db) << std::endl;
    }
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

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt);
        int userCount = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return userCount > 0;
    }
    sqlite3_finalize(stmt);
    return false;
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

void Database::addCard(int deckId, const std::string& question, const std::string& answer) {
    const char* sql = "INSERT INTO cards (deck_id, question, answer) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId);                           // Bind deckId
        sqlite3_bind_text(stmt, 2, question.c_str(), -1, SQLITE_TRANSIENT); // Bind question
        sqlite3_bind_text(stmt, 3, answer.c_str(), -1, SQLITE_TRANSIENT);   // Bind answer

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Card added successfully." << std::endl;
        } else {
            std::cerr << "Error adding card: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }
}

void Database::listCards(int deckId) {
    const char* sql = "SELECT id, question, answer FROM cards WHERE deck_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId); // Bind deckId

        std::cout << "Cards in deck " << deckId << ":" << std::endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cardId = sqlite3_column_int(stmt, 0);
            const char* question = (const char*)sqlite3_column_text(stmt, 1);
            const char* answer = (const char*)sqlite3_column_text(stmt, 2);

            std::cout << "  Card ID: " << cardId << ", Question: " << question << ", Answer: " << answer << std::endl;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }
}

void Database::startStudySession(int userId, int deckId) {
    const char* sql = "INSERT INTO study_sessions (user_id, deck_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, deckId);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Study session started for deck " << deckId << "." << std::endl;
    } else {
        std::cerr << "Error starting study session: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}


void Database::listStudySessions(int userId) {
    const char* sql = "SELECT s.id, d.name FROM study_sessions s JOIN decks d ON s.deck_id = d.id WHERE s.user_id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);

    std::cout << "Active study sessions for user " << userId << ":" << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int sessionId = sqlite3_column_int(stmt, 0);
        const char* deckName = (const char*)sqlite3_column_text(stmt, 1);
        std::cout << "  Session ID: " << sessionId << ", Deck: " << deckName << std::endl;
    }
    sqlite3_finalize(stmt);
}

std::vector<std::tuple<int, std::string, std::string>> Database::getDueCards(int userId, int deckId) {
    const char* sql = R"(
        SELECT c.id, c.question, c.answer
        FROM cards c
        JOIN card_progress cp ON c.id = cp.card_id
        WHERE cp.user_id = ? AND cp.deck_id = ? AND cp.due_date <= CURRENT_TIMESTAMP;
    )";
    sqlite3_stmt* stmt;

    std::vector<std::tuple<int, std::string, std::string>> dueCards;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, deckId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int cardId = sqlite3_column_int(stmt, 0);
        const char* question = (const char*)sqlite3_column_text(stmt, 1);
        const char* answer = (const char*)sqlite3_column_text(stmt, 2);
        dueCards.emplace_back(cardId, question, answer);
    }
    sqlite3_finalize(stmt);
    return dueCards;
}

void Database::updateCardProgress(int userId, int cardId, int interval, double easeFactor, int repetitions, int lapses) {
    const char* sql = R"(
        UPDATE card_progress
        SET interval = ?, ease_factor = ?, repetitions = ?, lapses = ?, due_date = DATETIME('now', '+' || ? || ' days')
        WHERE user_id = ? AND card_id = ?;
    )";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, interval);
    sqlite3_bind_double(stmt, 2, easeFactor);
    sqlite3_bind_int(stmt, 3, repetitions);
    sqlite3_bind_int(stmt, 4, lapses);
    sqlite3_bind_int(stmt, 5, interval);
    sqlite3_bind_int(stmt, 6, userId);
    sqlite3_bind_int(stmt, 7, cardId);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Card progress updated." << std::endl;
    } else {
        std::cerr << "Error updating card progress: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}

int Database::getUserId(const std::string& username) {
    const char* sql = "SELECT id FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    int userId = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userId = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    return userId;
}

