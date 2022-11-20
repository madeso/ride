#include "catch.hpp"

#include "base/rect.h"


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
    auto r = Recti{1, 2, 3, 4};
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
    // cut

    SECTION("cut left")
    {
        const auto cut = r.cut_left(1);
        CHECK(cut.get_left() == 1);
        CHECK(cut.get_right() == 2);
        CHECK(cut.get_bottom() == 2);
        CHECK(cut.get_top() == 6);

        CHECK(r.get_left() == 2);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("cut right")
    {
        const auto cut = r.cut_right(1);
        CHECK(cut.get_left() == 3);
        CHECK(cut.get_right() == 4);
        CHECK(cut.get_bottom() == 2);
        CHECK(cut.get_top() == 6);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 3);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("cut top")
    {
        const auto cut = r.cut_top(1);
        CHECK(cut.get_left() == 1);
        CHECK(cut.get_right() == 4);
        CHECK(cut.get_bottom() == 5);
        CHECK(cut.get_top() == 6);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 5);
    }

    SECTION("cut bottom")
    {
        const auto cut = r.cut_bottom(1);
        CHECK(cut.get_left() == 1);
        CHECK(cut.get_right() == 4);
        CHECK(cut.get_bottom() == 2);
        CHECK(cut.get_top() == 3);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 3);
        CHECK(r.get_top() == 6);
    }

    ///////////////////////////////////////////////////////////////////////////
    // get_cut
    SECTION("get cut left")
    {
        const auto cut = r.get_cut_left(1);
        CHECK(cut.get_left() == 1);
        CHECK(cut.get_right() == 2);
        CHECK(cut.get_bottom() == 2);
        CHECK(cut.get_top() == 6);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("get cut right")
    {
        const auto cut = r.get_cut_right(1);
        CHECK(cut.get_left() == 3);
        CHECK(cut.get_right() == 4);
        CHECK(cut.get_bottom() == 2);
        CHECK(cut.get_top() == 6);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("get cut top")
    {
        const auto cut = r.get_cut_top(1);
        CHECK(cut.get_left() == 1);
        CHECK(cut.get_right() == 4);
        CHECK(cut.get_bottom() == 5);
        CHECK(cut.get_top() == 6);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("get cut bottom")
    {
        const auto cut = r.get_cut_bottom(1);
        CHECK(cut.get_left() == 1);
        CHECK(cut.get_right() == 4);
        CHECK(cut.get_bottom() == 2);
        CHECK(cut.get_top() == 3);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    ///////////////////////////////////////////////////////////////////////////
    // get_offset

    SECTION("get offset (0,0)")
    {
        const auto off = r.get_offset({0, 0});
        CHECK(off.get_left() == 1);
        CHECK(off.get_right() == 4);
        CHECK(off.get_bottom() == 2);
        CHECK(off.get_top() == 6);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

    SECTION("get offset (...)")
    {
        const auto off = r.get_offset({1, 2});
        CHECK(off.get_left() == 2);
        CHECK(off.get_right() == 5);
        CHECK(off.get_bottom() == 4);
        CHECK(off.get_top() == 8);

        CHECK(r.get_left() == 1);
        CHECK(r.get_right() == 4);
        CHECK(r.get_bottom() == 2);
        CHECK(r.get_top() == 6);
    }

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


