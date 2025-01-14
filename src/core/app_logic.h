#ifndef APP_LOGIC_H
#define APP_LOGIC_H

#include "database.h"
#include "file/file_handler.h"
#include <string>
#include <vector>

class AppLogic {
public:
    AppLogic(Database& db);

    bool addUser(const std::string& username, const std::string& password);
    bool verifyUser(const std::string& username, const std::string& password);

    void createDeck(int userId, const std::string& deckName);
    void listDecks(int userId);
    bool deleteDeck(int userId, int deckId);
    bool importDeck(int userId, const std::string& filePath);

    void addCard(int deckId, const std::string& question, const std::string& answer);
    void listCards(int deckId);
    int getUserId(const std::string& username);

    void studyDeck(int userId, int deckId);
    std::vector<std::tuple<int, std::string, std::string>> getDueCards(int userId, int deckId);
    void updateCardProgress(int userId, int cardId, int grade);
    bool doesUserOwnDeck(int userId, int deckId);


private:
    Database& db;
    FileHandler fileHandler;
};

#endif
