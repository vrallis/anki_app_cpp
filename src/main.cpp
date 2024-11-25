#include "database.h"
#include "database_initializer.h"
#include <iostream>

int main() {
    Database db("anki_app.db");

    if (!DatabaseInitializer::isDatabaseInitialized(db)) {
        std::cout << "First-time setup detected. Initializing database..." << std::endl;
        DatabaseInitializer::initialize(db);
    }

    // Proceed with app logic
    return 0;
}
