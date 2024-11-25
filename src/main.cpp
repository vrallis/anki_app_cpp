#include "database.h"
#include <iostream>

int main() {
    std::cout << "Welcome to Wannabe Anki LOL" << std::endl;

    Database db("anki_app.db");

    db.addFirstUser();

    std::cout << "Initialization complete!" << std::endl;

    // TODO: Add CLI loop

    return 0;
}
