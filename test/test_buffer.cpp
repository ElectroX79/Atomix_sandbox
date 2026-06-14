#include "buffer.hpp"
#include "catch_amalgamated.hpp"


TEST_CASE("Buffer: operations", "[Buffer]"){
    Buffer<int> buff(500);

    SECTION("Insertion"){
        buff.push_back(20);
        REQUIRE(buff.size() == 1);
    }

    SECTION("Borrado") {
        buff.push_back(10);
        buff.erase(0);
        REQUIRE(buff.size() == 0);
    }
}

