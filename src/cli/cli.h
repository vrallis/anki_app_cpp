#ifndef CLI_H
#define CLI_H

#include "core/app_logic.h"

class CLI {
public:
    CLI(AppLogic& app);
    void run();

private:
    AppLogic& app;
    void mainMenu();
    void userMenu(int userId);
    void deckMenu(int userId);
    void handleUserCreation();
    void handleLogin();
    void handleStudySession(int userId);
};

#endif
