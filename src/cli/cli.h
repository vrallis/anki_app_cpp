#ifndef CLI_H
#define CLI_H

#include "core/app_logic.h"
#include "sound/sound_manager.h"
#include <nlohmann/json.hpp>

/**
 * @class CLI
 * @brief Command Line Interface for the application.
 * 
 * This class provides the command line interface for interacting with the application.
 * It handles user input and manages the flow of the application through various menus.
 */
 
/**
 * @brief Constructs a new CLI object.
 * 
 * @param app Reference to the application logic.
 */
 
/**
 * @brief Runs the CLI.
 * 
 * This function starts the command line interface and manages the main loop.
 */
 
/**
 * @brief Displays the main menu.
 * 
 * This function presents the main menu options to the user.
 */
 
/**
 * @brief Handles user creation.
 * 
 * This function manages the process of creating a new user.
 */
 
/**
 * @brief Handles user login.
 * 
 * This function manages the process of logging in an existing user.
 */
 
/**
 * @brief Displays the user menu.
 * 
 * @param userId The ID of the logged-in user.
 * 
 * This function presents the user-specific menu options.
 */
 
/**
 * @brief Displays the deck menu.
 * 
 * @param userId The ID of the logged-in user.
 * 
 * This function presents the deck management options to the user.
 */
 
/**
 * @brief Manages cards within a deck.
 * 
 * @param deckId The ID of the deck.
 * 
 * This function allows the user to manage cards within the specified deck.
 */
 
/**
 * @brief Handles a study session.
 * 
 * @param userId The ID of the logged-in user.
 * 
 * This function manages the study session for the user.
 */
 
/**
 * @brief Checks the next review date for a deck.
 * 
 * @param deckId The ID of the deck.
 * 
 * This function checks and displays the next review date for the specified deck.
 */
 
/**
 * @brief Loads the configuration.
 * 
 * This function loads the configuration settings for the CLI.
 */
class CLI {
public:
    CLI(AppLogic& app);
    void run();

private:
    void mainMenu();
    void handleUserCreation();
    void handleLogin();
    void userMenu(int userId);
    void deckMenu(int userId);
    void manageCards(int deckId);
    void handleStudySession(int userId);
    void checkNextReviewDate(int deckId);
    void loadConfig();

    AppLogic& app;
    SoundManager soundManager;
    static bool running;
    bool enableSounds;
};

#endif // CLI_H