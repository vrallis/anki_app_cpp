#ifndef DATABASE_INITIALIZER_H
#define DATABASE_INITIALIZER_H

#include "database.h"

class DatabaseInitializer {
public:
    static void initialize(Database& db);
    static bool isDatabaseInitialized(Database& db);
};

#endif
