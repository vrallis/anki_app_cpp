#include "database.h"
#include "app_logic.h"
#include "cli/cli.h"
#include "database_initializer.h"

int main() {
    Database db("anki_app.db");

    AppLogic appLogic(db);
    CLI cli(appLogic);
    cli.run();

    return 0;
}
