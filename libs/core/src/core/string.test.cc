#include "catch.hpp"

#include "catchy/stringeq.h"

#include "core/string.h"

using namespace catchy;


TEST_CASE("strings-whitespace-test", "[strings]")
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


TEST_CASE("strings-supersplit", "[strings]")
{
    CHECK(StringEq(supersplit("a,b,c", ','), {"a", "b", "c"}));
    CHECK(StringEq(supersplit("a,,c", ','), {"a", "", "c"}));
    CHECK(StringEq(supersplit("a,,,c", ','), {"a", "", "", "c"}));
    CHECK(StringEq(supersplit(",abc,", ','), {"", "abc", ""}));
    CHECK(StringEq(supersplit("", ','), {""}));
    CHECK(StringEq(supersplit(",", ','), {"", ""}));
}

TEST_CASE("strings-trim", "[strings]")
{
    CHECK(StringEq(trim(""), ""));
    CHECK(StringEq(trim(" abc "), "abc"));
    CHECK(StringEq(trim(" a b c "), "a b c"));

    const std::string only_space = " ";

    CHECK(StringEq(trim_left("", only_space), ""));
    CHECK(StringEq(trim_left(" abc ", only_space), "abc "));
    CHECK(StringEq(trim_left(" a b c ", only_space), "a b c "));

    CHECK(StringEq(trim_right("", only_space), ""));
    CHECK(StringEq(trim_right(" abc ", only_space), " abc"));
    CHECK(StringEq(trim_right(" a b c ", only_space), " a b c"));
}


TEST_CASE("strings-lower", "[strings]")
{
    CHECK(StringEq(to_lower(""), ""));
    CHECK(StringEq(to_lower("abc"), "abc"));
    CHECK(StringEq(to_lower("ABC"), "abc"));
    CHECK(StringEq(to_lower(" aBc "), " abc "));
}


TEST_CASE("strings-replace-all", "[strings]")
{
    CHECK(StringEq(replace_all_f("cut", "u", "a"), "cat"));
    CHECK(StringEq(replace_all_f("aaah, dogs!", "a", "o"), "oooh, dogs!"));
    CHECK(StringEq(replace_all_f("Look a horse, I like horses.", "horse", "cat"), "Look a cat, I like cats."));
    CHECK(StringEq(replace_all_f("Look a horse, I like horses.", "horse", "c++ compiler"), "Look a c++ compiler, I like c++ compilers."));
}

