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
        userMenu(userId);
    } else {
        std::cout << "Invalid credentials.\n";
    }
}

void CLI::userMenu(int userId) {
    while (true) {
        std::cout << "\n--- User Menu ---\n";
        std::cout << "1. Deck Management\n";
        std::cout << "2. Start Study Session\n";
        std::cout << "3. Logout\n";

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            deckMenu(userId);
            break;
        case 2:
            handleStudySession(userId);
            break;
        case 3:
            return;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void CLI::deckMenu(int userId) {
    while (true) {
        std::cout << "\n--- Deck Management ---\n";
        std::cout << "1. Create Deck\n";
        std::cout << "2. Delete Deck\n";
        std::cout << "3. Import Deck\n";
        std::cout << "4. List Decks\n";
        std::cout << "5. Go Back\n";

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string deckName;
            std::cout << "Enter deck name: ";
            std::cin >> deckName;
            app.createDeck(userId, deckName);
            std::cout << "Deck created successfully.\n";
            break;
        }
        case 2: {
            int deckId;
            std::cout << "Enter deck ID to delete: ";
            std::cin >> deckId;
            if (app.deleteDeck(userId, deckId)) {
                std::cout << "Deck deleted successfully.\n";
            } else {
                std::cout << "Failed to delete deck.\n";
            }
            break;
        }
        case 3: {
            std::string filePath;
            std::cout << "Enter file path for deck import: ";
            std::cin >> filePath;
            if (app.importDeck(userId, filePath)) {
                std::cout << "Deck imported successfully.\n";
            } else {
                std::cout << "Failed to import deck.\n";
            }
            break;
        }
        case 4:
            app.listDecks(userId);
            break;
        case 5:
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
