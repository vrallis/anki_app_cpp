#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <tuple>
#include <sqlite3.h>
#include "user.h"
#include <ctime>

/**
 * @class Database
 * @brief Manages the SQLite database connection and provides methods for user, deck, card, and study session management.
 * 
 * The Database class is responsible for connecting to an SQLite database and providing various methods to manage users, decks, cards, and study sessions. It also includes methods for spaced repetition and card progress updates.
 * 
 * @note This class requires the SQLite3 library.
 * 
 * @fn Database::Database(const std::string& db_name)
 * @brief Constructs a Database object with the specified database name.
 * @param db_name The name of the database to connect to.
 * 
 * @fn Database::~Database()
 * @brief Destroys the Database object and closes the database connection.
 * 
 * @fn sqlite3* Database::getDb() const
 * @brief Retrieves the SQLite database connection.
 * @return sqlite3* Pointer to the SQLite database connection.
 * 
 * @fn bool Database::addUser(const User& user)
 * @brief Adds a new user to the database.
 * @param user The user object containing user details.
 * @return bool True if the user was added successfully, false otherwise.
 * 
 * @fn bool Database::verifyUser(const std::string& username, const std::string& password)
 * @brief Verifies a user's credentials.
 * @param username The username of the user.
 * @param password The password of the user.
 * @return bool True if the credentials are correct, false otherwise.
 * 
 * @fn int Database::getUserId(const std::string& username)
 * @brief Retrieves the user ID for a given username.
 * @param username The username of the user.
 * @return int The user ID.
 * 
 * @fn int Database::createDeck(int userId, const std::string& deckName)
 * @brief Creates a new deck for a user.
 * @param userId The ID of the user.
 * @param deckName The name of the deck.
 * @return int The ID of the created deck.
 * 
 * @fn void Database::listDecks(int userId)
 * @brief Lists all decks for a user.
 * @param userId The ID of the user.
 * 
 * @fn bool Database::hasUsers()
 * @brief Checks if there are any users in the database.
 * @return bool True if there are users, false otherwise.
 * 
 * @fn void Database::startStudySession(int userId, int deckId)
 * @brief Starts a study session for a user and deck.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * 
 * @fn void Database::listStudySessions(int userId)
 * @brief Lists all study sessions for a user.
 * @param userId The ID of the user.
 * 
 * @fn std::vector<std::tuple<int, std::string, std::string>> Database::getDueCards(int userId, int deckId)
 * @brief Retrieves the cards that are due for review for a user and deck.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return std::vector<std::tuple<int, std::string, std::string>> A vector of tuples containing card ID, question, and answer.
 * 
 * @fn void Database::updateCardProgress(int userId, int cardId, int interval, double easeFactor, int repetitions, int lapses)
 * @brief Updates the progress of a card for spaced repetition.
 * @param userId The ID of the user.
 * @param cardId The ID of the card.
 * @param interval The interval until the next review.
 * @param easeFactor The ease factor for the card.
 * @param repetitions The number of repetitions.
 * @param lapses The number of lapses.
 * 
 * @fn bool Database::userOwnsDeck(int userId, int deckId)
 * @brief Checks if a user owns a specific deck.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return bool True if the user owns the deck, false otherwise.
 * 
 * @fn void Database::deleteDeck(int deckId)
 * @brief Deletes a deck from the database.
 * @param deckId The ID of the deck to delete.
 * 
 * @fn bool Database::addCard(int deckId, const std::string& question, const std::string& answer)
 * @brief Adds a new card to a deck.
 * @param deckId The ID of the deck.
 * @param question The question of the card.
 * @param answer The answer of the card.
 * @return bool True if the card was added successfully, false otherwise.
 * 
 * @fn bool Database::deleteCard(int cardId)
 * @brief Deletes a card from the database.
 * @param cardId The ID of the card to delete.
 * @return bool True if the card was deleted successfully, false otherwise.
 * 
 * @fn bool Database::editCard(int cardId, const std::string& question, const std::string& answer)
 * @brief Edits an existing card in the database.
 * @param cardId The ID of the card to edit.
 * @param question The new question of the card.
 * @param answer The new answer of the card.
 * @return bool True if the card was edited successfully, false otherwise.
 * 
 * @fn void Database::listCards(int deckId)
 * @brief Lists all cards in a deck.
 * @param deckId The ID of the deck.
 * 
 * @fn time_t Database::getNextReviewDate(int deckId)
 * @brief Retrieves the next review date for a deck.
 * @param deckId The ID of the deck.
 * @return time_t The next review date.
 * 
 * @fn void Database::executeSQL(const char* sql, const std::string& successMessage)
 * @brief Executes an SQL statement and prints a success message.
 * @param sql The SQL statement to execute.
 * @param successMessage The success message to print.
 */
class Database {
public:
    explicit Database(const std::string& db_name);
    ~Database();

    sqlite3* getDb() const { return db; }

    // User methods
    bool addUser(const User& user);
    bool verifyUser(const std::string& username, const std::string& password);
    int getUserId(const std::string& username);

    // Deck methods
    int createDeck(int userId, const std::string& deckName);
    void listDecks(int userId);

    // Card methods
    bool hasUsers();

    // Study session methods
    void startStudySession(int userId, int deckId);
    void listStudySessions(int userId);

    // Spaced repetition methods
   std::vector<std::tuple<int, std::string, std::string>> getDueCards(int userId, int deckId);
    void updateCardProgress(int userId, int cardId, int interval, double easeFactor, int repetitions, int lapses);

    bool userOwnsDeck(int userId, int deckId);
    void deleteDeck(int deckId);

    // card management
    bool addCard(int deckId, const std::string& question, const std::string& answer);
    bool deleteCard(int cardId);
    bool editCard(int cardId, const std::string& question, const std::string& answer);
    void listCards(int deckId);

    time_t getNextReviewDate(int deckId);

private:
    sqlite3* db;

    void executeSQL(const char* sql, const std::string& successMessage);

    friend class DatabaseInitializer;
};

#endif
