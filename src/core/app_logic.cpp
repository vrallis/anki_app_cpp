#include "app_logic.h"
#include <iostream>

AppLogic::AppLogic(Database& db) : db(db) {}

bool AppLogic::addUser(const std::string& username, const std::string& password) {
    db.addUser(User(username, password));
    return true;
}


bool AppLogic::verifyUser(const std::string& username, const std::string& password) {
    return db.verifyUser(username, password);
}

void AppLogic::createDeck(int userId, const std::string& deckName) {
    db.createDeck(userId, deckName);
}

void AppLogic::listDecks(int userId) {
    db.listDecks(userId);
}

void AppLogic::addCard(int deckId, const std::string& question, const std::string& answer) {
    db.addCard(deckId, question, answer);
}

void AppLogic::listCards(int deckId) {
    db.listCards(deckId);
}

int AppLogic::getUserId(const std::string& username) {
    return db.getUserId(username);
}

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
            interval *= easeFactor;
        }

        db.updateCardProgress(userId, cardId, interval, easeFactor, repetitions, lapses);
    }
}


