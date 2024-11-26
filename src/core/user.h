#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(const std::string& username, const std::string& password)
        : username(username), password(password) {}

    const std::string& getUsername() const { return username; }
    const std::string& getPassword() const { return password; }

private:
    std::string username;
    std::string password;
};

#endif
