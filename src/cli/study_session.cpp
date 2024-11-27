#include "study_session.h"
#include <iostream>

StudySession::StudySession(AppLogic& app, int userId, int deckId)
    : app(app), userId(userId), deckId(deckId) {}

void StudySession::start() {
    auto dueCards = fetchDueCards();

    if (dueCards.empty()) {
        std::cout << "No cards are due for review in this deck." << std::endl;
        return;
    }

    for (const auto& [cardId, question, answer] : dueCards) {
        reviewCard(cardId, question, answer);
    }
}

std::vector<std::tuple<int, std::string, std::string>> StudySession::fetchDueCards() {
    return app.getDueCards(userId, deckId); // Fetch due cards via AppLogic
}

void StudySession::reviewCard(int cardId, const std::string& question, const std::string& answer) {
    std::cout << "\nQuestion: " << question << std::endl;
    std::cout << "Enter your answer: ";
    std::string userAnswer;
    std::cin.ignore();
    std::getline(std::cin, userAnswer);

    std::cout << "Correct Answer: " << answer << std::endl;
    std::cout << "Rate your response (0=Forgot, 1=Hard, 2=Good, 3=Easy): ";
    int grade;
    std::cin >> grade;

    app.updateCardProgress(userId, cardId, grade); // Update progress via AppLogic
}
