#include "database.h"
#include "app_logic.h"
#include "cli/cli.h"
#include "database_initializer.h"

/**
 * @brief Entry point of the application.
 *
 * This function initializes the database, application logic, and command-line interface (CLI),
 * then starts the CLI to run the application.
 *
 * @return int Returns 0 upon successful execution.
 */
int main() {
    Database db("anki_app.db");

    AppLogic appLogic(db);
    CLI cli(appLogic);
    cli.run();

    return 0;
}
