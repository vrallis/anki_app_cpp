#ifndef USER_H
#define USER_H

#include <string>

/**
 * @class User
 * @brief Represents a user with a username and password.
 *
 * The User class encapsulates the details of a user, including their
 * username and password. It provides methods to retrieve these details.
 */

/**
 * @brief Constructs a new User object.
 *
 * @param username The username of the user.
 * @param password The password of the user.
 */

/**
 * @brief Gets the username of the user.
 *
 * @return const std::string& The username of the user.
 */

/**
 * @brief Gets the password of the user.
 *
 * @return const std::string& The password of the user.
 */
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
