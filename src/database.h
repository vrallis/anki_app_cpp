#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

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

class Database {
public:
    explicit Database(const std::string& db_name);
    ~Database();

    void initialize();
    void addFirstUser();
    void addUser(const User& user);
    bool hasUsers();

private:
    sqlite3* db;

    void executeSQL(const char* sql, const std::string& successMessage);
};

#endif
