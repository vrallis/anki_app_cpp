#ifndef CLI_H
#define CLI_H

#include "core/app_logic.h"

class CLI {
public:
    explicit CLI(AppLogic& app);

    void run();

private:
    AppLogic& app;

    void mainMenu();
    void handleUserCreation();
    void handleLogin();
    void handleDeckManagement(int userId);
    void handleStudySession(int userId);
};

#endif
