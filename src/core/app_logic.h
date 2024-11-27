#ifndef APP_LOGIC_H
#define APP_LOGIC_H

#include "database.h"

class AppLogic {
public:
    explicit AppLogic(Database& db);

    // User 
    bool addUser(const std::string& username, const std::string& password);
    bool verifyUser(const std::string& username, const std::string& password);

    // Deck functionality
    void createDeck(int userId, const std::string& deckName);
    void listDecks(int userId);

    // Card functionality
    void addCard(int deckId, const std::string& question, const std::string& answer);
    void listCards(int deckId);

    void studyDeck(int userId, int deckId);
    int getUserId(const std::string& username);

private:
    Database& db;
};

#endif
