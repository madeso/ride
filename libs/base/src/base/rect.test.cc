#include "catch.hpp"

#include "base/rect.h"

using R = recti;

TEST_CASE("rect-test", "[rect]")
{
    // rect at x,y  w,h
    /*
      (1,6)      (4,6)
        -----------
        |         |
        |         | h=4
        |   w=3   |
        -----------
      (1,2)       (4, 2)
    */
    R r = R{1, 2, 3, 4};
    CHECK(r.x == 1);
    CHECK(r.y == 2);
    CHECK(r.width == 3);
    CHECK(r.height == 4);

    CHECK(r.get_left() == 1);
    CHECK(r.get_right() == 4);
    CHECK(r.get_bottom() == 2);
    CHECK(r.get_top() == 6);

    ///////////////////////////////////////////////////////////////////////////
    // set_foobar(...)

    SECTION("set top")
    {
        r.set_top(42);
        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 42);
    }

    SECTION("set left")
    {
        r.set_left(-42);
        CHECK(r.get_left() == -42);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("set right")
    {
        r.set_right(42);
        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 42);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("set bottom")
    {
        r.set_bottom(-42);
        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == -42);
        CHECK(r.get_top() == 6);
    }

    ///////////////////////////////////////////////////////////////////////////
    // @todo(Gustav)

    ///////////////////////////////////////////////////////////////////////////
    // get_inset

    SECTION("inset 1")
    {
        const auto rr = r.get_inset(1);
        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);

        CHECK(rr.get_left() == 2);
        CHECK(rr.get_right() == 3);
        CHECK(rr.get_bottom() == 3);
        CHECK(rr.get_top() == 5);
    }

    SECTION("inset -1")
    {
        const auto rr = r.get_inset(-1);
        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);

        CHECK(rr.get_left() == 0);
        CHECK(rr.get_right() == 5);
        CHECK(rr.get_bottom() == 1);
        CHECK(rr.get_top() == 7);
    }
}


