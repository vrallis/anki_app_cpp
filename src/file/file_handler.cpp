#include "file_handler.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::pair<std::string, std::string>> FileHandler::importCards(const std::string& filePath) {
    std::vector<std::pair<std::string, std::string>> cards;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return cards;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string question, answer;

        if (std::getline(ss, question, ',') && std::getline(ss, answer, ',')) {
            cards.emplace_back(question, answer);
        } else {
            std::cerr << "Invalid format in line: " << line << std::endl;
        }
    }

    file.close();
    return cards;
}