#include "file_handler.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Imports cards from a specified file.
 *
 * This function reads a file containing card data, where each line represents a card
 * with a question and an answer separated by a comma. It parses the file and stores
 * the cards in a vector of pairs, where each pair consists of a question and an answer.
 *
 * @param filePath The path to the file containing the card data.
 * @return A vector of pairs, where each pair contains a question and an answer.
 *         If the file cannot be opened or if there are lines with invalid format,
 *         the function will return an empty vector or a partially filled vector.
 */
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