#include <catch2/catch.hpp>
#include "core/app_logic.h"
#include "core/database.h"
#include "test_utils.h"

TEST_CASE("AppLogic AddUser", "[AppLogic]") {
    setupTestDatabase();
    Database db("test.db");
    AppLogic app(db);
    REQUIRE(app.addUser("testuser", "password"));
    REQUIRE_FALSE(app.addUser("testuser", "password"));
}