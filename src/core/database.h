#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <tuple>
#include <sqlite3.h>
#include "user.h"

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
    void addCard(int deckId, const std::string& question, const std::string& answer);
    void listCards(int deckId);
    bool hasUsers();

    // Study session methods
    void startStudySession(int userId, int deckId);
    void listStudySessions(int userId);

    // Spaced repetition methods
    std::vector<std::tuple<int, std::string, std::string>> getDueCards(int userId, int deckId);
    void updateCardProgress(int userId, int cardId, int interval, double easeFactor, int repetitions, int lapses);

    bool userOwnsDeck(int userId, int deckId);
    void deleteDeck(int deckId);




private:
    sqlite3* db;

    void executeSQL(const char* sql, const std::string& successMessage);

    friend class DatabaseInitializer;
};

#endif
