#ifndef DATABASE_INITIALIZER_H
#define DATABASE_INITIALIZER_H

#include "database.h"

/**
 * @class DatabaseInitializer
 * @brief A utility class for initializing and checking the state of a database.
 *
 * This class provides static methods to initialize a database and to check if a database
 * has already been initialized.
 */

/**
 * @brief Initializes the given database.
 *
 * This method sets up the necessary tables and structures in the provided database.
 *
 * @param db The database to initialize.
 */
static void initialize(Database& db);

/**
 * @brief Checks if the given database has been initialized.
 *
 * This method checks if the necessary tables and structures have been set up in the provided database.
 *
 * @param db The database to check.
 * @return True if the database has been initialized, false otherwise.
 */
static bool isDatabaseInitialized(Database& db);
class DatabaseInitializer {
public:
    static void initialize(Database& db);
    static bool isDatabaseInitialized(Database& db);
};

#endif
