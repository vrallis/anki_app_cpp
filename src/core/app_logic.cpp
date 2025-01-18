#include "app_logic.h"
#include <iostream>

/**
 * @brief Constructs an AppLogic object with a reference to a Database object.
 * 
 * @param db A reference to a Database object that will be used by the AppLogic.
 */
AppLogic::AppLogic(Database& db) : db(db) {}

/**
 * @brief Adds a new user to the application.
 * 
 * This function creates a new user with the provided username and password,
 * and adds the user to the database.
 * 
 * @param username The username of the new user.
 * @param password The password of the new user.
 * @return true if the user was successfully added to the database, false otherwise.
 */
bool AppLogic::addUser(const std::string& username, const std::string& password) {
    return db.addUser(User(username, password));
}

/**
 * @brief Verifies the user's credentials.
 * 
 * This function checks the provided username and password against the database
 * to verify if the user exists and the credentials are correct.
 * 
 * @param username The username of the user to be verified.
 * @param password The password of the user to be verified.
 * @return true if the user credentials are valid, false otherwise.
 */
bool AppLogic::verifyUser(const std::string& username, const std::string& password) {
    return db.verifyUser(username, password);
}

/**
 * @brief Creates a new deck for a user.
 * 
 * This function creates a new deck in the database for the specified user.
 * 
 * @param userId The ID of the user for whom the deck is being created.
 * @param deckName The name of the deck to be created.
 */
void AppLogic::createDeck(int userId, const std::string& deckName) {
    db.createDeck(userId, deckName);
}

/**
 * @brief Lists all decks for a given user.
 * 
 * This function retrieves and lists all the decks associated with the specified user ID.
 * 
 * @param userId The ID of the user whose decks are to be listed.
 */
void AppLogic::listDecks(int userId) {
    db.listDecks(userId);
}

/**
 * @brief Deletes a deck owned by a user.
 *
 * This function checks if the specified user owns the deck with the given deck ID.
 * If the user owns the deck, it deletes the deck from the database and returns true.
 * Otherwise, it logs an error message and returns false.
 *
 * @param userId The ID of the user attempting to delete the deck.
 * @param deckId The ID of the deck to be deleted.
 * @return true if the deck was successfully deleted, false otherwise.
 */
bool AppLogic::deleteDeck(int userId, int deckId) {
    if (db.userOwnsDeck(userId, deckId)) {
        db.deleteDeck(deckId);
        return true;
    } else {
        std::cerr << "Error: User " << userId << " does not own deck " << deckId << "." << std::endl;
        return false;
    }
}

/**
 * @brief Imports a deck of cards from a specified file and associates it with a user.
 *
 * This function reads card data from the provided file path, prompts the user to enter a name for the new deck,
 * creates the deck in the database, and adds the imported cards to the newly created deck.
 *
 * @param userId The ID of the user who is importing the deck.
 * @param filePath The path to the file containing the card data to be imported.
 * @return true if the deck was successfully imported and added to the database, false otherwise.
 */
bool AppLogic::importDeck(int userId, const std::string& filePath) {
    auto cards = fileHandler.importCards(filePath);
    if (cards.empty()) {
        std::cerr << "No valid cards to import from file: " << filePath << std::endl;
        return false;
    }

    std::string deckName;
    std::cout << "Enter a name for the imported deck: ";
    std::cin >> deckName;

    int deckId = db.createDeck(userId, deckName);
    if (deckId == -1) {
        std::cerr << "Failed to create deck." << std::endl;
        return false;
    }

    for (const auto& [question, answer] : cards) {
        db.addCard(deckId, question, answer);
    }

    std::cout << "Deck '" << deckName << "' imported successfully with " << cards.size() << " cards.\n";
    return true;
}

/**
 * @brief Adds a new card to the specified deck.
 * 
 * This function adds a new card with the given question and answer to the deck
 * identified by the provided deck ID.
 * 
 * @param deckId The ID of the deck to which the card will be added.
 * @param question The question text for the new card.
 * @param answer The answer text for the new card.
 */
void AppLogic::addCard(int deckId, const std::string& question, const std::string& answer) {
    db.addCard(deckId, question, answer);
}

/**
 * @brief Edits the content of a card in the database.
 * 
 * This function updates the question and answer of a card identified by its ID.
 * 
 * @param cardId The unique identifier of the card to be edited.
 * @param newQuestion The new question text for the card.
 * @param newAnswer The new answer text for the card.
 */
void AppLogic::editCard(int cardId, const std::string& newQuestion, const std::string& newAnswer) {
    db.editCard(cardId, newQuestion, newAnswer);
}

/**
 * @brief Removes a card from the database.
 * 
 * This function deletes a card from the database using the provided card ID.
 * 
 * @param cardId The ID of the card to be removed.
 */
void AppLogic::removeCard(int cardId) {
    db.deleteCard(cardId);
}

/**
 * @brief Lists all cards in the specified deck.
 * 
 * This function retrieves and lists all the cards that belong to the deck
 * identified by the given deck ID.
 * 
 * @param deckId The ID of the deck whose cards are to be listed.
 */
void AppLogic::listCards(int deckId) {
    db.listCards(deckId);
}

/**
 * @brief Retrieves the user ID associated with the given username.
 * 
 * This function queries the database to find the user ID corresponding
 * to the provided username.
 * 
 * @param username The username for which to retrieve the user ID.
 * @return The user ID associated with the given username.
 */
int AppLogic::getUserId(const std::string& username) {
    return db.getUserId(username);
}

/**
 * @brief Checks if a user owns a specific deck.
 * 
 * This function queries the database to determine if the user with the given
 * userId owns the deck with the given deckId.
 * 
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return true if the user owns the deck, false otherwise.
 */
bool AppLogic::doesUserOwnDeck(int userId, int deckId) {
    return db.userOwnsDeck(userId, deckId);
}

/**
 * @brief Retrieves the next review date for a given deck.
 * 
 * This function queries the database to get the next review date
 * for the deck specified by the deckId parameter.
 * 
 * @param deckId The ID of the deck for which to retrieve the next review date.
 * @return The next review date as a time_t value.
 */
time_t AppLogic::getNextReviewDate(int deckId) {
    return db.getNextReviewDate(deckId);
}

/**
 * @brief Conducts a study session for a specific deck for a given user.
 *
 * This function retrieves the due cards for the specified user and deck,
 * presents each card's question to the user, and prompts the user to input
 * their answer. After displaying the correct answer, the user is asked to
 * rate their response. Based on the rating, the function updates the card's
 * progress in the database, adjusting the interval, ease factor, repetitions,
 * and lapses accordingly.
 *
 * @param userId The ID of the user studying the deck.
 * @param deckId The ID of the deck being studied.
 */
void AppLogic::studyDeck(int userId, int deckId) {
    auto dueCards = db.getDueCards(userId, deckId);
    for (const auto& [cardId, question, answer] : dueCards) {
        std::cout << "Question: " << question << std::endl;
        std::cout << "Enter your answer: ";
        std::string userAnswer;
        std::cin.ignore();
        std::getline(std::cin, userAnswer);

        std::cout << "Correct Answer: " << answer << std::endl;
        std::cout << "Rate your response (0=Forgot, 1=Hard, 2=Good, 3=Easy): ";
        int grade;
        std::cin >> grade;

        int interval = 1;
        double easeFactor = 2.5;
        int repetitions = 0;
        int lapses = 0;

        if (grade == 0) {
            lapses++;
            interval = 1;
        } else {
            repetitions++;
            easeFactor += (0.1 - (3 - grade) * (0.08 + (3 - grade) * 0.02));
            easeFactor = std::max(1.3, easeFactor); // drop factor need to not be infinite
            interval *= easeFactor;
        }

        db.updateCardProgress(userId, cardId, interval, easeFactor, repetitions, lapses);
    }
}

/**
 * @brief Retrieves a list of due cards for a specific user and deck.
 * 
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 * @return A vector of tuples, where each tuple contains:
 *         - An integer representing the card ID.
 *         - A string representing the card question.
 *         - A string representing the card answer.
 */
std::vector<std::tuple<int, std::string, std::string>> AppLogic::getDueCards(int userId, int deckId) {
    return db.getDueCards(userId, deckId);
}

/**
 * @brief Updates the progress of a card for a specific user based on the grade provided.
 *
 * This function updates the interval, ease factor, repetitions, and lapses of a card
 * based on the grade given by the user. The grade must be between 0 and 3, inclusive.
 * If the grade is 0, it indicates a lapse and the interval is reset to 1. For grades
 * 1 to 3, the ease factor is adjusted and the interval is updated accordingly.
 *
 * @param userId The ID of the user.
 * @param cardId The ID of the card.
 * @param grade The grade given by the user (must be between 0 and 3).
 */
void AppLogic::updateCardProgress(int userId, int cardId, int grade) {
    if (grade < 0 || grade > 3) {
        std::cerr << "Invalid grade: " << grade << ". Grade must be between 0 and 3." << std::endl;
        return;
    }

    int interval = 1;
    double easeFactor = 2.5;
    int repetitions = 0;
    int lapses = 0;

    if (grade == 0) {
        lapses++;
        interval = 1;
    } else {
        repetitions++;
        easeFactor += (0.1 - (3 - grade) * (0.08 + (3 - grade) * 0.02));
        easeFactor = std::max(1.3, easeFactor); // drop factor need to not be infinite
        interval *= easeFactor;
    }

    db.updateCardProgress(userId, cardId, interval, easeFactor, repetitions, lapses);
}