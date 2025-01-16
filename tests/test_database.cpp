#include <catch2/catch.hpp>
#include "core/database.h"
#include "core/user.h"
#include "test_utils.h"

TEST_CASE("Database AddUser", "[Database]") {
    setupTestDatabase();
    Database db("test.db");
    User user("testuser", "password");
    REQUIRE(db.addUser(user));
    REQUIRE_FALSE(db.addUser(user));
}