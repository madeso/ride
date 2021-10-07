#include "catch.hpp"

#include "base/string.h"

TEST_CASE("strings-test", "[strings]")
{
    CHECK(is_only_whitepace(" "));
    CHECK(is_only_whitepace("\n\t"));
    CHECK(is_only_whitepace("\r\t"));
    CHECK_FALSE(is_only_whitepace("abc"));
    CHECK_FALSE(is_only_whitepace("abc"));
    
    CHECK(index_of_non_whitespace("abc") == 0);
    CHECK(index_of_non_whitespace(" abc") == 1);
    CHECK(index_of_non_whitespace("a b c") == 0);
}
