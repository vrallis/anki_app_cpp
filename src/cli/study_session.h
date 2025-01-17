#ifndef STUDY_SESSION_H
#define STUDY_SESSION_H

#include "core/app_logic.h"
#include "sound/sound_manager.h"
#include <vector>
#include <tuple>

class StudySession {
public:
    StudySession(AppLogic& app, SoundManager& soundManager, int userId, int deckId);

    void start();

private:
    std::vector<std::tuple<int, std::string, std::string>> fetchDueCards();
    void reviewCard(int cardId, const std::string& question, const std::string& answer);

    AppLogic& app;
    SoundManager& soundManager;
    int userId;
    int deckId;
};

#endif // STUDY_SESSION_H