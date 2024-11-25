#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>
#include "user.h"

class Database {
public:
    explicit Database(const std::string& db_name);
    ~Database();

    void initialize();

    // User methods
    void addUser(const User& user);
    bool verifyUser(const std::string& username, const std::string& password);

    // Deck methods
    void createDeck(int userId, const std::string& deckName);
    void listDecks(int userId);

    // Card methods
    void addCard(int deckId, const std::string& question, const std::string& answer);
    void listCards(int deckId);
    bool hasUsers();


private:
    sqlite3* db;

    void executeSQL(const char* sql, const std::string& successMessage);

    friend class DatabaseInitializer;
};

#endif
