#include "catch.hpp"

#include "catchy/stringeq.h"

#include "core/cc.h"

using namespace catchy;

TEST_CASE("cc-splice", "[cc]")
{
    using L = std::vector<std::string>;

    SECTION("Remove 0 (zero) elements before index 2, and insert drum")
    {
        L myFish = {"angel", "clown", "mandarin", "sturgeon"};
        L removed = splice(&myFish, 2, 0, {"drum"});
        CHECK(StringEq(myFish, {"angel", "clown", "drum", "mandarin", "sturgeon"}));
        CHECK(StringEq(removed, {})); // no elements removed
    }

    SECTION("Remove 0 (zero) elements before index 2, and insert drum and guitar")
    {
        L myFish = {"angel", "clown", "mandarin", "sturgeon"};
        L removed = splice(&myFish, 2, 0, {"drum", "guitar"});
        CHECK(StringEq(myFish, {"angel", "clown", "drum", "guitar", "mandarin", "sturgeon"}));
        CHECK(StringEq(removed, {})); // no elements removed
    }

    SECTION("Remove 1 element at index 3")
    {
        L myFish = {"angel", "clown", "drum", "mandarin", "sturgeon"};
        L removed = splice(&myFish, 3, 1);
        CHECK(StringEq(myFish, {"angel", "clown", "drum", "sturgeon"}));
        CHECK(StringEq(removed, {"mandarin"}));
    }

    SECTION("Remove 1 element at index 2, and insert trumpet")
    {
        L myFish = {"angel", "clown", "drum", "sturgeon"};
        L removed = splice(&myFish, 2, 1, {"trumpet"});
        CHECK(StringEq(myFish, {"angel", "clown", "trumpet", "sturgeon"}));
        CHECK(StringEq(removed, {"drum"}));
    }

    SECTION("Remove 2 elements from index 0, and insert parrot, anemone and blue")
    {
        L myFish = {"angel", "clown", "trumpet", "sturgeon"};
        L removed = splice(&myFish, 0, 2, {"parrot", "anemone", "blue"});
        CHECK(StringEq(myFish, {"parrot", "anemone", "blue", "trumpet", "sturgeon"}));
        CHECK(StringEq(removed, {"angel", "clown"}));
    }

    SECTION("Remove 2 elements, starting from index 2")
    {
        L myFish = {"parrot", "anemone", "blue", "trumpet", "sturgeon"};
        L removed = splice(&myFish, 2, 2);
        CHECK(StringEq(myFish, {"parrot", "anemone", "sturgeon"}));
        CHECK(StringEq(removed, {"blue", "trumpet"}));
    }

    SECTION("Remove 1 element from index -2")
    {
        L myFish = {"angel", "clown", "mandarin", "sturgeon"};
        L removed = splice(&myFish, -2, 1);
        CHECK(StringEq(myFish, {"angel", "clown", "sturgeon"}));
        CHECK(StringEq(removed, {"mandarin"}));
    }

    SECTION("Remove all elements, starting from index 2")
    {
        L myFish = {"angel", "clown", "mandarin", "sturgeon"};
        L removed = splice(&myFish, 2);
        CHECK(StringEq(myFish, {"angel", "clown"}));
        CHECK(StringEq(removed, {"mandarin", "sturgeon"}));
    }
}
