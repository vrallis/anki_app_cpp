#include "cli.h"
#include "core/database.h"
#include <iostream>
#include <string>
#include "study_session.h"


CLI::CLI(AppLogic& app) : app(app) {}

void CLI::run() {
    while (true) {
        mainMenu();
    }
}

void CLI::mainMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Create User\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";

    int choice;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
    case 1:
        handleUserCreation();
        break;
    case 2:
        handleLogin();
        break;
    case 3:
        std::cout << "Exiting...\n";
        exit(0);
    default:
        std::cout << "Invalid choice. Try again.\n";
    }
}

void CLI::handleUserCreation() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (app.addUser(username, password)) {
        std::cout << "User created successfully.\n";
    } else {
        std::cout << "Failed to create user. User might already exist.\n";
    }
}

void CLI::handleLogin() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (app.verifyUser(username, password)) {
        std::cout << "Login successful.\n";
        int userId = app.getUserId(username);
        handleDeckManagement(userId);
    } else {
        std::cout << "Invalid credentials.\n";
    }
}

void CLI::handleDeckManagement(int userId) {
    while (true) {
        std::cout << "\n--- Deck Management ---\n";
        std::cout << "1. Create Deck\n";
        std::cout << "2. List Decks\n";
        std::cout << "3. Go Back\n";

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string deckName;
            std::cout << "Enter deck name: ";
            std::cin >> deckName;
            app.createDeck(userId, deckName);
            break;
        }
        case 2:
            app.listDecks(userId);
            break;
        case 3:
            return;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void CLI::handleStudySession(int userId) {
    int deckId;
    std::cout << "Enter the deck ID to study: ";
    std::cin >> deckId;

    StudySession session(app, userId, deckId);
    session.start();
}

