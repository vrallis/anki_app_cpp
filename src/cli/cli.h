#ifndef CLI_H
#define CLI_H

#include "core/app_logic.h"

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
    void handleStudySession(int userId);
    void manageCards(int deckId);

    AppLogic& app;
};

#endif // CLI_H