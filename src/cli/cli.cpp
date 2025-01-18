#include "cli.h"
#include "core/database.h"
#include "ascii_art.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "study_session.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool CLI::running = true;

/**
 * @brief Constructs a new CLI object.
 * 
 * This constructor initializes the CLI object with the given application logic,
 * sets the default value for enabling sounds, and loads the configuration.
 * If sounds are enabled, it attempts to load the necessary sound files.
 * 
 * @param app Reference to the application logic object.
 */
CLI::CLI(AppLogic& app) : app(app), enableSounds(true) {
    loadConfig();
    // DEBUG
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (enableSounds) {
        if (!soundManager.loadSound("src/assets/sounds/click.wav", "click")) {
            std::cerr << "Error loading click sound." << std::endl;
        }
        if (!soundManager.loadSound("src/assets/sounds/correct.wav", "correct")) {
            std::cerr << "Error loading correct sound." << std::endl;
        }
        if (!soundManager.loadSound("src/assets/sounds/incorrect.wav", "incorrect")) {
            std::cerr << "Error loading incorrect sound." << std::endl;
        }
    }
}

/**
 * @brief Loads the configuration settings from a JSON file.
 *
 * This function attempts to load the configuration settings from a file named
 * "config.json" located in the current working directory. If the file is found
 * and successfully opened, it reads the JSON content and updates the settings
 * accordingly. If the file cannot be opened, it prints an error message and
 * uses the default settings.
 *
 * @note The function expects the JSON file to contain a boolean value for the
 * key "enable_sounds". If the key is not present, the default value of true is used.
 */
void CLI::loadConfig() {
    std::filesystem::path exePath = std::filesystem::current_path();
    std::filesystem::path configPath = exePath / "config.json";

    std::ifstream configFile(configPath);
    if (configFile.is_open()) {
        json config;
        configFile >> config;
        enableSounds = config.value("enable_sounds", true);
    } else {
        std::cerr << "Could not open config file at " << configPath << ". Using default settings." << std::endl;
    }
}
/**
 * @brief Runs the command line interface (CLI) main loop.
 * 
 * This function keeps the CLI running by continuously displaying the main menu
 * as long as the `running` flag is set to true.
 */
void CLI::run() {
    while (running) {
        mainMenu();
    }
}

/**
 * @brief Displays the main menu and handles user input.
 * 
 * This function prints the main menu options to the console, prompts the user for input,
 * and processes the input accordingly. The available options are:
 * 1. Create User
 * 2. Login
 * 3. Exit
 * 
 * If the user enters an invalid input, an error message is displayed and the function returns.
 * If sounds are enabled, a click sound is played upon receiving input.
 * 
 * Depending on the user's choice, the function will:
 * - Call handleUserCreation() for option 1
 * - Call handleLogin() for option 2
 * - Set the running flag to false and print an exit message for option 3
 * - Print an error message for any other input
 */
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

    if (enableSounds) {
        soundManager.playSound("click");
    }

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

/**
 * @brief Handles the creation of a new user.
 *
 * Prompts the user to enter a username and password, then attempts to create
 * a new user with the provided credentials. If the user is created
 * successfully, a success message is displayed. If the user creation fails
 * (e.g., if the user already exists), an error message is displayed.
 */
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

/**
 * @brief Handles the login process for the CLI application.
 * 
 * Prompts the user to enter their username and password, and verifies the credentials.
 * If the credentials are valid, it proceeds to the user menu. Otherwise, it displays
 * an error message indicating invalid credentials.
 */
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

/**
 * @brief Displays the user menu and handles user input for various actions.
 *
 * This function continuously displays a menu with options for deck management,
 * starting a study session, and logging out. It processes user input and calls
 * the appropriate functions based on the user's choice. If the input is invalid,
 * it prompts the user to try again.
 *
 * @param userId The ID of the user for whom the menu is being displayed.
 */
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

        if (enableSounds) {
            soundManager.playSound("click");
        }

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

/**
 * @brief Displays the deck management menu and handles user input for various deck-related operations.
 * 
 * This function presents a menu to the user with options to create, delete, import, list decks,
 * manage cards within a deck, check the next review date for a deck, or go back to the previous menu.
 * It continuously runs until the user chooses to go back.
 * 
 * @param userId The ID of the user for whom the deck operations are to be performed.
 * 
 * Menu Options:
 * 1. Create Deck - Prompts the user to enter a deck name and creates a new deck.
 * 2. Delete Deck - Prompts the user to enter a deck ID and deletes the specified deck.
 * 3. Import Deck - Prompts the user to enter a file path and imports a deck from the specified file.
 * 4. List Decks - Lists all decks owned by the user.
 * 5. Manage Cards - Prompts the user to enter a deck ID and manages cards within the specified deck.
 * 6. Check Next Review Date - Prompts the user to enter a deck ID and checks the next review date for the specified deck.
 * 7. Go Back - Exits the deck management menu and returns to the previous menu.
 * 
 * If an invalid input is provided, the function will prompt the user to try again.
 * If sounds are enabled, a click sound will be played upon each valid menu selection.
 */
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

        if (enableSounds) {
            soundManager.playSound("click");
        }

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

/**
 * @brief Manages the cards within a specified deck.
 * 
 * This function provides a command-line interface for managing cards in a deck.
 * It allows the user to add, edit, delete, and list cards, as well as go back to the previous menu.
 * 
 * @param deckId The ID of the deck to manage.
 * 
 * The function displays a menu with the following options:
 * 1. Add Card: Prompts the user to enter a question and answer to create a new card.
 * 2. Edit Card: Prompts the user to enter the card ID, new question, and new answer to edit an existing card.
 * 3. Delete Card: Prompts the user to enter the card ID to delete an existing card.
 * 4. List Cards: Lists all cards in the specified deck.
 * 5. Go Back: Exits the card management menu and returns to the previous menu.
 * 
 * If an invalid input is entered, the function will prompt the user to try again.
 * If sounds are enabled, a click sound will be played upon each valid choice.
 */
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

        if (enableSounds) {
            soundManager.playSound("click");
        }

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

/**
 * @brief Handles the study session for a given user.
 * 
 * Prompts the user to enter the deck ID they wish to study. If the user owns the deck,
 * a new study session is created and started. If the user does not own the deck or it 
 * does not exist, an error message is displayed.
 * 
 * @param userId The ID of the user who wants to start a study session.
 */
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

/**
 * @brief Checks and prints the next review date for a given deck.
 * 
 * This function retrieves the next review date for the specified deck
 * and prints it to the standard output. If there are no cards in the
 * deck, it informs the user that the deck is empty.
 * 
 * @param deckId The ID of the deck to check the next review date for.
 */
void CLI::checkNextReviewDate(int deckId) {
    time_t nextReviewDate = app.getNextReviewDate(deckId);
    if (nextReviewDate == 0) {
        std::cout << "There are no cards in this deck." << std::endl;
    } else {
        std::cout << "The next review date for deck " << deckId << " is: " << std::ctime(&nextReviewDate);
    }
}