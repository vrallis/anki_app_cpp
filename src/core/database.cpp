#include "database.h"
#include <iostream>
#include <sqlite3.h>
#include "database_initializer.h"
#include <ctime>

/**
 * @brief Constructs a Database object and opens a connection to the specified SQLite database.
 *
 * This constructor attempts to open a connection to the SQLite database specified by `db_name`.
 * If the database cannot be opened, an error message is printed to the standard error stream.
 * If the database is opened successfully, it checks whether the database is initialized.
 * If the database is not initialized, it initializes the database.
 *
 * @param db_name The name of the SQLite database file.
 */
Database::Database(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        if (!DatabaseInitializer::isDatabaseInitialized(*this)) {
            std::cout << "Database not detected. Creating one right now!" << std::endl;
            DatabaseInitializer::initialize(*this);
        } else {
            std::cout << "Database opened successfully!" << std::endl;
        }
    }
}


/**
 * @brief Destructor for the Database class.
 *
 * This destructor is responsible for closing the SQLite database connection
 * by calling sqlite3_close on the database handle.
 */
Database::~Database() {
    sqlite3_close(db);
}

/**
 * @brief Adds a new user to the database.
 * 
 * This function attempts to insert a new user into the 'users' table in the database.
 * It binds the username and password from the provided User object to the SQL statement
 * and executes it. If the insertion is successful, the function returns true. If the
 * user already exists or any other error occurs, it returns false and logs the error.
 * 
 * @param user The User object containing the username and password to be added.
 * @return true if the user was successfully added to the database.
 * @return false if there was an error adding the user to the database.
 */
bool Database::addUser(const User& user) {
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    bool success = false;

    std::cout << "Attempting to add user: " << user.getUsername() << std::endl;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);

        int stepResult = sqlite3_step(stmt);
        if (stepResult == SQLITE_DONE) {
            std::cout << "User '" << user.getUsername() << "' added to the database.\n";
            success = true;
        } else {
            int errCode = sqlite3_errcode(db);
            std::cerr << "Error adding user: " << sqlite3_errmsg(db) << " (Error code: " << errCode << ")\n";
            if (errCode == SQLITE_CONSTRAINT_UNIQUE) {
                std::cerr << "Error: User '" << user.getUsername() << "' already exists.\n";
                success = false;
            }
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare addUser query: " << sqlite3_errmsg(db) << std::endl;
    }

    std::cout << "Add user result: " << (success ? "success" : "failure") << std::endl;
    return success;
}


/**
 * @brief Verifies the user's credentials by checking the username and password in the database.
 *
 * This function prepares and executes an SQL statement to check if a user with the given
 * username and password exists in the database. It returns true if a matching user is found,
 * otherwise false.
 *
 * @param username The username of the user to verify.
 * @param password The password of the user to verify.
 * @return true if the user exists and the credentials are correct, false otherwise.
 */
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

/**
 * @brief Creates a new deck for a specified user in the database.
 *
 * This function inserts a new deck into the 'decks' table with the given user ID and deck name.
 * It returns the ID of the newly created deck.
 *
 * @param userId The ID of the user who owns the deck.
 * @param deckName The name of the deck to be created.
 * @return The ID of the newly created deck, or -1 if an error occurred.
 */
int Database::createDeck(int userId, const std::string& deckName) {
    const char* sql = "INSERT INTO decks (user_id, name) VALUES (?, ?) RETURNING id;";
    sqlite3_stmt* stmt;
    int deckId = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_text(stmt, 2, deckName.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            deckId = sqlite3_column_int(stmt, 0);
        } else {
            std::cerr << "Error creating deck: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare createDeck query: " << sqlite3_errmsg(db) << std::endl;
    }

    return deckId;
}



/**
 * @brief Lists all decks for a given user.
 * 
 * This function queries the database for all decks associated with the specified user ID
 * and prints the deck ID and name for each deck.
 * 
 * @param userId The ID of the user whose decks are to be listed.
 */
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

/**
 * @brief Checks if there are any users in the database.
 *
 * This function executes a SQL query to count the number of rows in the 'users' table.
 * It returns true if there is at least one user, and false otherwise.
 *
 * @return true if there is at least one user in the database, false otherwise.
 */
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

/**
 * @brief Executes an SQL statement on the database.
 *
 * This function executes the provided SQL statement on the database and prints
 * a success message if the execution is successful. If there is an error during
 * execution, it prints the error message.
 *
 * @param sql The SQL statement to be executed.
 * @param successMessage The message to be printed if the SQL execution is successful.
 */
void Database::executeSQL(const char* sql, const std::string& successMessage) {
    char* errMsg = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error executing SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << successMessage << std::endl;
    }
}

/**
 * @brief Starts a study session for a given user and deck.
 *
 * This function inserts a new record into the `study_sessions` table with the provided
 * user ID and deck ID, indicating the start of a study session.
 *
 * @param userId The ID of the user starting the study session.
 * @param deckId The ID of the deck being studied.
 */
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


/**
 * @brief Lists the active study sessions for a given user.
 *
 * This function queries the database for all active study sessions associated with the specified user ID.
 * It retrieves the session ID and the name of the deck associated with each study session and prints them to the console.
 *
 * @param userId The ID of the user whose study sessions are to be listed.
 */
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

/**
 * @brief Checks if a user owns a specific deck.
 *
 * This function queries the database to determine if a deck with the given
 * deckId is owned by the user with the given userId.
 *
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return true if the user owns the deck, false otherwise.
 */
bool Database::userOwnsDeck(int userId, int deckId) {
    const char* sql = "SELECT COUNT(*) FROM decks WHERE id = ? AND user_id = ?;";
    sqlite3_stmt* stmt;
    bool ownsDeck = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId);
        sqlite3_bind_int(stmt, 2, userId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            ownsDeck = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare userOwnsDeck query: " << sqlite3_errmsg(db) << std::endl;
    }

    return ownsDeck;
}

/**
 * @brief Deletes a deck and all associated cards from the database.
 * 
 * This function deletes a deck from the 'decks' table and all cards associated 
 * with that deck from the 'cards' table in the database.
 * 
 * @param deckId The ID of the deck to be deleted.
 * 
 * The function prepares and executes an SQL statement to delete the deck and 
 * its associated cards. If the SQL statement is successfully prepared and 
 * executed, a success message is printed to the standard output. If there is 
 * an error during preparation or execution, an error message is printed to the 
 * standard error output.
 */
void Database::deleteDeck(int deckId) {
    const char* sql = "DELETE FROM cards WHERE deck_id = ?; DELETE FROM decks WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId);
        sqlite3_bind_int(stmt, 2, deckId);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error deleting deck: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Deck and associated cards deleted successfully." << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare deleteDeck query: " << sqlite3_errmsg(db) << std::endl;
    }
}

/**
 * @brief Adds a new card to the database.
 * 
 * This function inserts a new card into the 'cards' table with the specified deck ID, question, answer, 
 * and the current timestamp as the due date.
 * 
 * @param deckId The ID of the deck to which the card belongs.
 * @param question The question text of the card.
 * @param answer The answer text of the card.
 * @return true if the card was added successfully, false otherwise.
 */
bool Database::addCard(int deckId, const std::string& question, const std::string& answer) {
    const char* sql = "INSERT INTO cards (deck_id, question, answer, dueDate) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    bool success = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId);
        sqlite3_bind_text(stmt, 2, question.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, answer.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, std::time(nullptr));

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Card added successfully.\n";
            success = true;
        } else {
            std::cerr << "Error adding card: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare addCard query: " << sqlite3_errmsg(db) << std::endl;
    }

    return success;
}

/**
 * @brief Deletes a card from the database.
 * 
 * This function deletes a card from the 'cards' table in the database
 * based on the provided card ID. It prepares an SQL DELETE statement,
 * binds the card ID to the statement, and executes it. If the deletion
 * is successful, it returns true; otherwise, it returns false and logs
 * an error message.
 * 
 * @param cardId The ID of the card to be deleted.
 * @return true if the card was deleted successfully, false otherwise.
 */
bool Database::deleteCard(int cardId) {
    const char* sql = "DELETE FROM cards WHERE id = ?;";
    sqlite3_stmt* stmt;
    bool success = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, cardId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Card deleted successfully.\n";
            success = true;
        } else {
            std::cerr << "Error deleting card: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare deleteCard query: " << sqlite3_errmsg(db) << std::endl;
    }

    return success;
}

/**
 * @brief Edits a card in the database with the given card ID.
 *
 * This function updates the question and answer fields of a card in the database
 * identified by the specified card ID. It prepares an SQL statement to update
 * the card's information and executes it.
 *
 * @param cardId The ID of the card to be edited.
 * @param question The new question text for the card.
 * @param answer The new answer text for the card.
 * @return true if the card was successfully updated, false otherwise.
 */
bool Database::editCard(int cardId, const std::string& question, const std::string& answer) {
    const char* sql = "UPDATE cards SET question = ?, answer = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    bool success = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, question.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, answer.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, cardId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Card updated successfully.\n";
            success = true;
        } else {
            std::cerr << "Error updating card: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare editCard query: " << sqlite3_errmsg(db) << std::endl;
    }

    return success;
}

/**
 * @brief Lists all cards in a specified deck.
 *
 * This function retrieves and prints all cards from the database that belong to the specified deck.
 * Each card's ID, question, and answer are printed to the standard output.
 *
 * @param deckId The ID of the deck whose cards are to be listed.
 */
void Database::listCards(int deckId) {
    const char* sql = "SELECT id, question, answer FROM cards WHERE deck_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId);

        std::cout << "Cards in deck " << deckId << ":\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cardId = sqlite3_column_int(stmt, 0);
            const char* question = (const char*)sqlite3_column_text(stmt, 1);
            const char* answer = (const char*)sqlite3_column_text(stmt, 2);

            std::cout << "  Card ID: " << cardId << ", Question: " << question << ", Answer: " << answer << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare listCards query: " << sqlite3_errmsg(db) << std::endl;
    }
}

/**
 * @brief Updates the progress of a card in the database.
 *
 * This function updates the interval, ease factor, repetitions, lapses, and due date
 * of a card identified by its cardId for a specific user identified by userId.
 *
 * @param userId The ID of the user whose card progress is being updated.
 * @param cardId The ID of the card to update.
 * @param interval The new interval (in days) for the card.
 * @param easeFactor The new ease factor for the card.
 * @param repetitions The number of times the card has been reviewed successfully.
 * @param lapses The number of times the card has been forgotten.
 */
void Database::updateCardProgress(int userId, int cardId, int interval, double easeFactor, int repetitions, int lapses) {
    const char* sql = "UPDATE cards SET interval = ?, easeFactor = ?, repetitions = ?, lapses = ?, dueDate = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    bool success = false;

    int dueDate = std::time(nullptr) + interval * 24 * 60 * 60; 

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, interval);
        sqlite3_bind_double(stmt, 2, easeFactor);
        sqlite3_bind_int(stmt, 3, repetitions);
        sqlite3_bind_int(stmt, 4, lapses);
        sqlite3_bind_int(stmt, 5, dueDate);
        sqlite3_bind_int(stmt, 6, cardId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Card progress updated successfully.\n";
            success = true;
        } else {
            std::cerr << "Error updating card progress: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare updateCardProgress query: " << sqlite3_errmsg(db) << std::endl;
    }
}

/**
 * @brief Retrieves the due cards for a specific user and deck.
 *
 * This function queries the database to find all cards that are due for review
 * based on the current time. It returns a vector of tuples, where each tuple
 * contains the card ID, question, and answer.
 *
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return A vector of tuples, each containing the card ID, question, and answer
 *         of the due cards.
 */
std::vector<std::tuple<int, std::string, std::string>> Database::getDueCards(int userId, int deckId) {
    const char* sql = "SELECT id, question, answer FROM cards WHERE deck_id = ? AND dueDate <= ?;";
    sqlite3_stmt* stmt;
    std::vector<std::tuple<int, std::string, std::string>> dueCards;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId);
        sqlite3_bind_int(stmt, 2, std::time(nullptr));

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cardId = sqlite3_column_int(stmt, 0);
            const char* question = (const char*)sqlite3_column_text(stmt, 1);
            const char* answer = (const char*)sqlite3_column_text(stmt, 2);

            dueCards.emplace_back(cardId, question, answer);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare getDueCards query: " << sqlite3_errmsg(db) << std::endl;
    }

    return dueCards;
}

/**
 * @brief Retrieves the user ID for a given username from the database.
 *
 * This function prepares and executes an SQL query to select the user ID
 * from the users table where the username matches the provided input.
 *
 * @param username The username for which to retrieve the user ID.
 * @return The user ID associated with the given username, or -1 if the user
 *         is not found or if an error occurs during the query execution.
 */
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
        std::cerr << "Failed to prepare getUserId query: " << sqlite3_errmsg(db) << std::endl;
    }

    return userId;
}

/**
 * @brief Retrieves the next review date for a given deck.
 *
 * This function queries the database to find the minimum due date of cards
 * in the specified deck. It returns the earliest due date as a time_t value.
 *
 * @param deckId The ID of the deck for which to retrieve the next review date.
 * @return The next review date as a time_t value. If no cards are found, returns 0.
 */
time_t Database::getNextReviewDate(int deckId) {
    const char* sql = "SELECT MIN(dueDate) FROM cards WHERE deck_id = ?;";
    sqlite3_stmt* stmt;
    time_t nextReviewDate = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, deckId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            nextReviewDate = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare getNextReviewDate query: " << sqlite3_errmsg(db) << std::endl;
    }

    return nextReviewDate;
}
