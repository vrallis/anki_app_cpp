#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>

class FileHandler {
public:
    static std::vector<std::pair<std::string, std::string>> importCards(const std::string& filePath);
};

#endif // FILE_HANDLER_H