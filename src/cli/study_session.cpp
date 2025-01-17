#include "study_session.h"
#include <iostream>

StudySession::StudySession(AppLogic& app, SoundManager& soundManager, int userId, int deckId)
    : app(app), soundManager(soundManager), userId(userId), deckId(deckId) {}

void StudySession::start() {
    // Check if the user owns the deck
    if (!app.doesUserOwnDeck(userId, deckId)) {
        std::cerr << "Error: You do not have access to this deck or it does not exist." << std::endl;
        return;
    }

    // Fetch due cards
    auto dueCards = fetchDueCards();
    if (dueCards.empty()) {
        std::cout << "No cards are due for review in this deck." << std::endl;
        return;
    }

    // Review each due card
    for (const auto& [cardId, question, answer] : dueCards) {
        reviewCard(cardId, question, answer);
    }
}

std::vector<std::tuple<int, std::string, std::string>> StudySession::fetchDueCards() {
    return app.getDueCards(userId, deckId);
}

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