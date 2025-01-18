#ifndef STUDY_SESSION_H
#define STUDY_SESSION_H

#include "core/app_logic.h"
#include "sound/sound_manager.h"
#include <vector>
#include <tuple>

/**
 * @class StudySession
 * @brief Manages a study session for a user, handling the review of due cards.
 *
 * The StudySession class is responsible for managing the flow of a study session,
 * including fetching due cards and reviewing them.
 *
 * @param app Reference to the application's logic handler.
 * @param soundManager Reference to the sound manager for playing audio cues.
 * @param userId The ID of the user participating in the study session.
 * @param deckId The ID of the deck being studied.
 */
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