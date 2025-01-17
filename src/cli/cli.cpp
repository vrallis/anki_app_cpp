#include "cli.h"
#include "core/database.h"
#include "ascii_art.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "study_session.h"

bool CLI::running = true;

CLI::CLI(AppLogic& app) : app(app) {
    // Print the current working directory
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    // // Check if the sound files exist
    // if (!std::filesystem::exists("../src/assets/sounds/click.wav")) {
    //     std::cerr << "File not found: ../src/assets/sounds/click.wav" << std::endl;
    // }
    // if (!std::filesystem::exists("../src/assets/sounds/correct.wav")) {
    //     std::cerr << "File not found: ../src/assets/sounds/correct.wav" << std::endl;
    // }
    // if (!std::filesystem::exists("../src/assets/sounds/incorrect.wav")) {
    //     std::cerr << "File not found: ../src/assets/sounds/incorrect.wav" << std::endl;
    // }

    // if (!soundManager.loadSound("../src/assets/sounds/click.wav", "click")) {
    //     std::cerr << "Error loading click sound." << std::endl;
    // }
    // if (!soundManager.loadSound("../src/assets/sounds/correct.wav", "correct")) {
    //     std::cerr << "Error loading correct sound." << std::endl;
    // }
    // if (!soundManager.loadSound("../src/assets/sounds/incorrect.wav", "incorrect")) {
    //     std::cerr << "Error loading incorrect sound." << std::endl;
    // }
}

void CLI::run() {
    while (running) {
        mainMenu();
    }
}

void CLI::mainMenu() {
    std::cout << generateAsciiArt("Main Menu") << std::endl;
    std::cout << "1. Create User" << std::endl;
    std::cout << "2. Login" << std::endl;
    std::cout << "3. Exit" << std::endl;

    int choice;
    std::cout << "Enter your choice: ";
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Try again." << std::endl;
        return;
    }

    soundManager.playSound("click");

    switch (choice) {
    case 1:
        handleUserCreation();
        break;
    case 2:
        handleLogin();
        break;
    case 3:
        std::cout << "Exiting..." << std::endl;
        running = false;
        break;
    default:
        std::cout << "Invalid choice. Try again." << std::endl;
    }
}

void CLI::handleUserCreation() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (app.addUser(username, password)) {
        std::cout << "User created successfully." << std::endl;
    } else {
        std::cout << "Failed to create user. User might already exist." << std::endl;
    }
}

void CLI::handleLogin() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (app.verifyUser(username, password)) {
        std::cout << "Login successful." << std::endl;
        int userId = app.getUserId(username);
        userMenu(userId);
    } else {
        std::cout << "Invalid credentials." << std::endl;
    }
}

void CLI::userMenu(int userId) {
    while (running) {
        std::cout << generateAsciiArt("User Menu") << std::endl;
        std::cout << "1. Deck Management" << std::endl;
        std::cout << "2. Start Study Session" << std::endl;
        std::cout << "3. Logout" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again." << std::endl;
            continue;
        }

        soundManager.playSound("click");

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
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    }
}

void CLI::deckMenu(int userId) {
    while (running) {
        std::cout << generateAsciiArt("Deck Management") << std::endl;
        std::cout << "1. Create Deck" << std::endl;
        std::cout << "2. Delete Deck" << std::endl;
        std::cout << "3. Import Deck" << std::endl;
        std::cout << "4. List Decks" << std::endl;
        std::cout << "5. Manage Cards" << std::endl;
        std::cout << "6. Check Next Review Date" << std::endl;
        std::cout << "7. Go Back" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again." << std::endl;
            continue;
        }

        soundManager.playSound("click");

        switch (choice) {
        case 1: {
            std::string deckName;
            std::cout << "Enter deck name: ";
            std::cin >> deckName;
            app.createDeck(userId, deckName);
            std::cout << "Deck created successfully." << std::endl;
            break;
        }
        case 2: {
            int deckId;
            std::cout << "Enter deck ID to delete: ";
            std::cin >> deckId;
            if (app.deleteDeck(userId, deckId)) {
                std::cout << "Deck deleted successfully." << std::endl;
            } else {
                std::cout << "Failed to delete deck." << std::endl;
            }
            break;
        }
        case 3: {
            std::string filePath;
            std::cout << "Enter file path for deck import: ";
            std::cin >> filePath;
            if (app.importDeck(userId, filePath)) {
                std::cout << "Deck imported successfully." << std::endl;
            } else {
                std::cout << "Failed to import deck." << std::endl;
            }
            break;
        }
        case 4:
            app.listDecks(userId);
            break;
        case 5: {
            int deckId;
            std::cout << "Enter deck ID to manage cards: ";
            std::cin >> deckId;
            if (app.doesUserOwnDeck(userId, deckId)) {
                manageCards(deckId);
            } else {
                std::cout << "You do not own this deck or it does not exist." << std::endl;
            }
            break;
        }
        case 6: {
            int deckId;
            std::cout << "Enter deck ID to check next review date: ";
            std::cin >> deckId;
            checkNextReviewDate(deckId);
            break;
        }
        case 7:
            return;
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    }
}

void CLI::manageCards(int deckId) {
    while (running) {
        std::cout << generateAsciiArt("Manage Cards") << std::endl;
        std::cout << "1. Add Card" << std::endl;
        std::cout << "2. Edit Card" << std::endl;
        std::cout << "3. Delete Card" << std::endl;
        std::cout << "4. List Cards" << std::endl;
        std::cout << "5. Go Back" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again." << std::endl;
            continue;
        }

        soundManager.playSound("click");

        switch (choice) {
        case 1: {
            std::string question, answer;
            std::cout << "Enter question: ";
            std::cin.ignore();
            std::getline(std::cin, question);
            std::cout << "Enter answer: ";
            std::getline(std::cin, answer);
            app.addCard(deckId, question, answer);
            std::cout << "Card added successfully." << std::endl;
            break;
        }
        case 2: {
            int cardId;
            std::string question, answer;
            std::cout << "Enter card ID to edit: ";
            std::cin >> cardId;
            std::cout << "Enter new question: ";
            std::cin.ignore();
            std::getline(std::cin, question);
            std::cout << "Enter new answer: ";
            std::getline(std::cin, answer);
            app.editCard(cardId, question, answer);
            std::cout << "Card edited successfully." << std::endl;
            break;
        }
        case 3: {
            int cardId;
            std::cout << "Enter card ID to delete: ";
            std::cin >> cardId;
            app.removeCard(cardId);
            std::cout << "Card deleted successfully." << std::endl;
            break;
        }
        case 4:
            app.listCards(deckId);
            break;
        case 5:
            return;
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    }
}

void CLI::handleStudySession(int userId) {
    int deckId;
    std::cout << "Enter the deck ID to study: ";
    std::cin >> deckId;

    if (app.doesUserOwnDeck(userId, deckId)) {
        StudySession session(app, soundManager, userId, deckId);
        session.start();
    } else {
        std::cout << "You do not own this deck or it does not exist." << std::endl;
    }
}

void CLI::checkNextReviewDate(int deckId) {
    time_t nextReviewDate = app.getNextReviewDate(deckId);
    if (nextReviewDate == 0) {
        std::cout << "There are no cards in this deck." << std::endl;
    } else {
        std::cout << "The next review date for deck " << deckId << " is: " << std::ctime(&nextReviewDate);
    }
}