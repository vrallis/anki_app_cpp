#ifndef CLI_H
#define CLI_H

#include "core/app_logic.h"
#include "sound/sound_manager.h"

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

    AppLogic& app;
    SoundManager soundManager;
    static bool running;
};

#endif // CLI_H