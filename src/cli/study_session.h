#ifndef STUDY_SESSION_H
#define STUDY_SESSION_H

#include <string>
#include <vector>
#include <tuple>
#include "core/app_logic.h"


class StudySession {
public:
    StudySession(AppLogic& app, int userId, int deckId);

    void start();

private:
    AppLogic& app;
    int userId;
    int deckId;

    std::vector<std::tuple<int, std::string, std::string>> fetchDueCards();
    void reviewCard(int cardId, const std::string& question, const std::string& answer);
};

#endif
