#include "catch.hpp"

#include "catchy/stringeq.h"
#include "catchy/vectorequals.h"

#include "base/str.h"
#include "libride/document.h"
#include "libride/document.commands.h"
#include "libride/command.h"

using namespace catchy;

template<typename T>
FalseString
VectorEquals(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
    return VectorEquals
    (
        lhs,
        rhs,
        [](const T& f) -> std::string { return Str() << f; },
        [](const T& a, const T& b) -> FalseString
        {
            if(a == b)
            {
                return FalseString::True();
            }
            else
            {
                return FalseString::False
                (
                    Str() << a << " != " << b
                );
            }
        }
    );
}

// position tests
// selection tests

TEST_CASE("doc-get_char", "[doc]")
{
    Document doc;
    doc.lines =
    {
        "abc",
        "def",
        "ghi"
    };

    SECTION("inside")
    {
        CHECK(doc.get_char({0, 0}) == 'a');
        CHECK(doc.get_char({0, 1}) == 'b');
        CHECK(doc.get_char({1, 0}) == 'd');
        CHECK(doc.get_char({2, 0}) == 'g');
        CHECK(doc.get_char({2, 2}) == 'i');
    }

    SECTION("outside negative")
    {
        CHECK(doc.get_char({-1, 1}) == 'b');
        CHECK(doc.get_char({1, -1}) == 'd');
        CHECK(doc.get_char({-1, -1}) == 'a');
    }

    SECTION("outside positive")
    {
        const auto i = GENERATE(3, 4, 5);
        CAPTURE(i);

        CHECK(doc.get_char({0, i}) == '\n');
        CHECK(doc.get_char({i, 2}) == '\n');
        CHECK(doc.get_char({2, i}) == '\n');
        CHECK(doc.get_char({i, i}) == '\n');
    }
}

TEST_CASE("doc-get_line_at", "[doc]")
{
    Document doc;
    doc.lines =
    {
        "abc",
        "def"
    };

    CHECK(StringEq(doc.GetLineAt(0), "abc"));
    CHECK(StringEq(doc.GetLineAt(1), "def"));
}

TEST_CASE("doc-number-of-lines", "[doc]")
{
    Document doc;

    SECTION("one line")
    {
        doc.lines =
        {
            "a"
        };
        CHECK(doc.GetNumberOfLines() == 1);
    }

    SECTION("one line")
    {
        doc.lines =
        {
            "a",
            "b"
        };
        CHECK(doc.GetNumberOfLines() == 2);
    }
}

namespace
{
    struct TestView : VirtualView
    {
        void scroll_to_cursor(const position&) override
        {
        }

        double scale = 1.0;

        pix get_relative_pixel_offset(const position& p) override
        {
            return pix{static_cast<double>(p.offset) * scale};
        }

        int get_offset_from_relative_pixel_offset(int, pix offset) override
        {
            return static_cast<int>(offset.value / scale);
        }
    };
}

TEST_CASE("doc-virtual-view", "[doc]")
{
    auto doc = std::make_shared<Document>();

    CHECK(doc->views.size() == 0);

    SECTION("test view")
    {
        auto view = TestView{};
        view.set_document(doc);
        CHECK(doc->views.size() == 1);

        doc->lines =
        {
            "abc def",
            "ghi"
        };

        SECTION("insert char")
        {
            view.cursors =
            {
                {{0,0}, {0,0}}
            };
            view.insert_text_at_cursors("z");
            CHECK(StringEq(doc->GetLineAt(0), "zabc def"));
            CHECK(StringEq(doc->GetLineAt(1), "ghi"));
            // check cursors
            CHECK(VectorEquals(
                view.cursors,
                {
                    {{0,1}, {0,1}}
                }
            ));
        }

        SECTION("commands")
        {
            CommandList commands;

            add_edit_commands(&commands, [&](){ return &view;});

            SECTION("multi cursors on first line")
            {
                view.cursors =
                {
                    {{0,0}, {0,0}},
                    {{0,3}, {0,3}}
                };

                SECTION("move to end")
                {
                    CHECK(commands.perform("doc.move-end"));
                    CHECK(VectorEquals(
                        view.cursors,
                        {
                            {{0,position::max_offset}, {0,position::max_offset}}
                        }
                    ));
                }
            }
        }
    }

    CHECK(doc->views.size() == 0);
}
