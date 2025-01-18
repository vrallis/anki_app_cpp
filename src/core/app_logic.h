#ifndef APP_LOGIC_H
#define APP_LOGIC_H

#include "database.h"
#include "file/file_handler.h"
#include <string>
#include <vector>

/**
 * @class AppLogic
 * @brief Handles the core logic of the application, including user management, deck management, and card management.
 * 
 * @param db Reference to the Database object for database operations.
 * 
 * @fn AppLogic::AppLogic(Database& db)
 * @brief Constructor that initializes the AppLogic with a reference to the database.
 * 
 * @fn bool AppLogic::addUser(const std::string& username, const std::string& password)
 * @brief Adds a new user to the database.
 * @param username The username of the new user.
 * @param password The password of the new user.
 * @return True if the user was added successfully, false otherwise.
 * 
 * @fn bool AppLogic::verifyUser(const std::string& username, const std::string& password)
 * @brief Verifies the user's credentials.
 * @param username The username of the user.
 * @param password The password of the user.
 * @return True if the credentials are correct, false otherwise.
 * 
 * @fn void AppLogic::createDeck(int userId, const std::string& deckName)
 * @brief Creates a new deck for the specified user.
 * @param userId The ID of the user.
 * @param deckName The name of the new deck.
 * 
 * @fn void AppLogic::listDecks(int userId)
 * @brief Lists all decks for the specified user.
 * @param userId The ID of the user.
 * 
 * @fn bool AppLogic::deleteDeck(int userId, int deckId)
 * @brief Deletes a deck for the specified user.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck to be deleted.
 * @return True if the deck was deleted successfully, false otherwise.
 * 
 * @fn bool AppLogic::importDeck(int userId, const std::string& filePath)
 * @brief Imports a deck from a file for the specified user.
 * @param userId The ID of the user.
 * @param filePath The path to the file containing the deck.
 * @return True if the deck was imported successfully, false otherwise.
 * 
 * @fn void AppLogic::addCard(int deckId, const std::string& question, const std::string& answer)
 * @brief Adds a new card to the specified deck.
 * @param deckId The ID of the deck.
 * @param question The question of the new card.
 * @param answer The answer of the new card.
 * 
 * @fn void AppLogic::editCard(int cardId, const std::string& newQuestion, const std::string& newAnswer)
 * @brief Edits an existing card.
 * @param cardId The ID of the card to be edited.
 * @param newQuestion The new question for the card.
 * @param newAnswer The new answer for the card.
 * 
 * @fn void AppLogic::removeCard(int cardId)
 * @brief Removes a card from the deck.
 * @param cardId The ID of the card to be removed.
 * 
 * @fn void AppLogic::listCards(int deckId)
 * @brief Lists all cards in the specified deck.
 * @param deckId The ID of the deck.
 * 
 * @fn int AppLogic::getUserId(const std::string& username)
 * @brief Retrieves the user ID for the specified username.
 * @param username The username of the user.
 * @return The user ID.
 * 
 * @fn void AppLogic::studyDeck(int userId, int deckId)
 * @brief Initiates a study session for the specified deck.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * 
 * @fn std::vector<std::tuple<int, std::string, std::string>> AppLogic::getDueCards(int userId, int deckId)
 * @brief Retrieves the cards that are due for review in the specified deck.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return A vector of tuples containing the card ID, question, and answer of the due cards.
 * 
 * @fn void AppLogic::updateCardProgress(int userId, int cardId, int grade)
 * @brief Updates the progress of a card based on the user's performance.
 * @param userId The ID of the user.
 * @param cardId The ID of the card.
 * @param grade The grade given by the user.
 * 
 * @fn bool AppLogic::doesUserOwnDeck(int userId, int deckId)
 * @brief Checks if the specified user owns the specified deck.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return True if the user owns the deck, false otherwise.
 * 
 * @fn time_t AppLogic::getNextReviewDate(int deckId)
 * @brief Retrieves the next review date for the specified deck.
 * @param deckId The ID of the deck.
 * @return The next review date as a time_t object.
 */
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
    void editCard(int cardId, const std::string& newQuestion, const std::string& newAnswer);
    void removeCard(int cardId);
    void listCards(int deckId);
    int getUserId(const std::string& username);

    void studyDeck(int userId, int deckId);
    std::vector<std::tuple<int, std::string, std::string>> getDueCards(int userId, int deckId);
    void updateCardProgress(int userId, int cardId, int grade);
    bool doesUserOwnDeck(int userId, int deckId);

    time_t getNextReviewDate(int deckId);

private:
    Database& db;
    FileHandler fileHandler;
};

#endif // APP_LOGIC_H