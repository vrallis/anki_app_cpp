#include "study_session.h"
#include <iostream>

/**
 * @brief Constructs a new StudySession object.
 * 
 * @param app Reference to the AppLogic instance.
 * @param soundManager Reference to the SoundManager instance.
 * @param userId The ID of the user.
 * @param deckId The ID of the deck.
 */
StudySession::StudySession(AppLogic& app, SoundManager& soundManager, int userId, int deckId)
    : app(app), soundManager(soundManager), userId(userId), deckId(deckId) {}

/**
 * @brief Starts a study session for the user.
 *
 * This function initiates a study session by first checking if the user has access to the specified deck.
 * If the user does not own the deck or the deck does not exist, an error message is displayed and the function returns.
 * If the user has access, it fetches the due cards for review. If there are no due cards, a message is displayed and the function returns.
 * Otherwise, it proceeds to review each due card by calling the reviewCard function.
 */
void StudySession::start() {
    if (!app.doesUserOwnDeck(userId, deckId)) {
        std::cerr << "Error: You do not have access to this deck or it does not exist." << std::endl;
        return;
    }

    auto dueCards = fetchDueCards();
    if (dueCards.empty()) {
        std::cout << "No cards are due for review in this deck." << std::endl;
        return;
    }

    for (const auto& [cardId, question, answer] : dueCards) {
        reviewCard(cardId, question, answer);
    }
}

/**
 * @brief Fetches the due cards for the current study session.
 *
 * This function retrieves a list of due cards for the user and deck associated
 * with the current study session. Each card is represented as a tuple containing
 * the card ID, the question, and the answer.
 *
 * @return A vector of tuples, where each tuple contains:
 *         - int: The card ID.
 *         - std::string: The question text.
 *         - std::string: The answer text.
 */
std::vector<std::tuple<int, std::string, std::string>> StudySession::fetchDueCards() {
    return app.getDueCards(userId, deckId);
}

/**
 * @brief Reviews a flashcard by presenting the question, accepting the user's answer, and rating the response.
 * 
 * This function displays the question to the user, prompts them to enter their answer, and then shows the correct answer.
 * The user is then asked to rate their response on a scale from 0 to 3, where:
 * - 0 = Forgot
 * - 1 = Hard
 * - 2 = Good
 * - 3 = Easy
 * 
 * The function updates the card's progress based on the user's rating and plays a sound indicating whether the response was correct or incorrect.
 * 
 * @param cardId The ID of the card being reviewed.
 * @param question The question text of the card.
 * @param answer The correct answer text of the card.
 */
void StudySession::reviewCard(int cardId, const std::string& question, const std::string& answer) {
    std::cout << "\nQuestion: " << question << std::endl;
    std::cout << "Enter your answer: ";
    std::string userAnswer;
    std::cin.ignore();
    std::getline(std::cin, userAnswer);

    std::cout << "Correct Answer: " << answer << std::endl;

    int grade;
    while (true) {
        std::cout << "Rate your response (0=Forgot, 1=Hard, 2=Good, 3=Easy): ";
        std::cin >> grade;

        if (grade >= 0 && grade <= 3) {
            break;
        } else {
            std::cout << "Invalid grade. Please enter a number between 0 and 3.\n";
        }
    }

    app.updateCardProgress(userId, cardId, grade);

    if (grade > 0) {
        soundManager.playSound("correct");
    } else {
        soundManager.playSound("incorrect");
    }
}