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

    std::vector<std::tuple<int, std::string, std::string>> getDueCards(int userId, int deckId);
    void updateCardProgress(int userId, int cardId, int grade);


private:
    Database& db;
};

#endif
