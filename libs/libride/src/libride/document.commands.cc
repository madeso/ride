#include "libride/document.commands.h"

#include <iostream>
#include <string_view>

#include "base/string.h"
#include "base/utf8.h"
#include "base/c.h"

#include "libride/document.h"
#include "libride/command.h"

using is_word_fun = std::function<bool (char)>;

position previous_char(std::shared_ptr<Document> doc, const position& pp)
{
    auto p = pp;
    do
    {
        p = doc->position_offset(p, -1);
    }
    while(is_utf8_cont_char(doc->get_char(p)));
    return p;
}


position next_char(std::shared_ptr<Document> doc, const position& pp)
{
    auto p = pp;
    do
    {
        p = doc->position_offset(p, 1);
    } while(is_utf8_cont_char(doc->get_char(p)));
    return p;
}


position previous_word_start(std::shared_ptr<Document> doc, const position& pp, const is_word_fun& is_word_char)
{
    auto p = pp;

    while((p.line >= 0 || p.offset>=0) && !is_word_char(doc->get_char(doc->position_offset(p, -1))) )
    {
        const auto last = p;
        p = doc->position_offset(p, -1);
        if(p == last) { return p; }
    }

    // skip line if it isn't whitespace
    while((p.line >= 0 || p.offset>=0) && is_word_char(doc->get_char(doc->position_offset(p, -1))) )
    {
        const auto last = p;
        p = doc->position_offset(p, -1);
        if(p == last) { return p; }
    }

    return p;
}


position next_word_end(std::shared_ptr<Document> doc, const position& pp, const is_word_fun& is_word_char)
{
    auto p = pp;

    while(!is_word_char(doc->get_char(p)) )
    {
        const auto last = p;
        p = doc->position_offset(p, 1);
        if(p == last) { return p; }
    }

    while(is_word_char(doc->get_char(p)) )
    {
        const auto last = p;
        p = doc->position_offset(p, 1);
        if(p == last) { return p; }
    }

    return p;
}


position start_of_word(std::shared_ptr<Document> doc, const position& pp, const is_word_fun& is_word_char)
{
    auto p = pp;
    while((p.line >= 0 || p.offset>=0) && is_word_char(doc->get_char(doc->position_offset(p, -1))) )
    {
        const auto last = p;
        p = doc->position_offset(p, -1);
        if(p == last) { return p; }
    }
    return p;
}


position end_of_word(std::shared_ptr<Document> doc, const position& pp, const is_word_fun& is_word_char)
{
    auto p = pp;
    while(is_word_char(doc->get_char(p)) )
    {
        const auto last = p;
        p = doc->position_offset(p, 1);
        if(p == last) { return p; }
    }
    return p;
}


position previous_block_start(std::shared_ptr<Document> doc, const position& pp)
{
    auto p = pp;

    const auto line_at = [doc](int line) -> std::string { return doc->GetLineAt(line); };

    // skip line if it is whitespace
    while(p.line > 0 && is_only_whitepace(line_at(p.line)) )
    {
        p.line -= 1;
    }

    // skip line if it isn't whitespace
    while(p.line > 0 && !is_only_whitepace(line_at(p.line)) )
    {
        p.line -= 1;
    }

    p.offset = index_of_non_whitespace(line_at(p.line));

    return p;
}


position next_block_end(std::shared_ptr<Document> doc, const position& pp)
{
    auto p = pp;

    const auto lc = doc->GetNumberOfLines();
    const auto line_at = [doc](int line) -> std::string { return doc->GetLineAt(line); };

    // skip line if it is whitespace
    while(p.line < lc && is_only_whitepace(line_at(p.line)) )
    {
        p.line += 1;
    }

    // skip line if it isn't whitespace
    while(p.line < lc && !is_only_whitepace(line_at(p.line)) )
    {
        p.line += 1;
    }

    p.offset = index_of_non_whitespace(line_at(p.line));

    return p;
}


position start_of_line(const position& p)
{
    return {p.line, 0};
}


position end_of_line(const position& p)
{
    return {p.line, position::max_offset};
}


position start_of_doc()
{
    return {0, 0};
}


position end_of_doc(std::shared_ptr<Document> doc)
{
    return {doc->GetNumberOfLines(), position::max_offset};
}

position change_line(VirtualView* view, const position& p, int change)
{
    const auto new_line = view->doc->sanitize_line(p.line + change);
    if(new_line == p.line) { return p; }

    const auto old_pixel = view->get_relative_pixel_offset(p);
    const auto new_offset = view->get_offset_from_relative_pixel_offset(new_line, old_pixel);
    std::cout << "old pixel " << old_pixel.value << " -> " << new_offset << "\n";
    return {new_line, new_offset};
}


///////////////////////////////////////////////////////////////////////////////////////////////////

bool is_whitespace(char c)
{
    switch(c)
    {
    case ' ':
    case '\t':
    case '\n':
        return true;
    default: return false;
    }
}

void add_edit_commands(CommandList* list, active_view_or_null_getter get_view)
{
    auto add_edit_command = list->add
    (
        [get_view]() -> bool
        {
            auto* view = get_view();
            return view != nullptr && view->doc != nullptr;
        }
    );

    add_edit_command
    (
        "doc.insert-newline",
        [get_view]()
        {
            auto* view = get_view();
            if(view == nullptr) { return; }

            view->insert_text_at_cursors("\n");
        }
    );

    auto add_complex_command = [get_view, &add_edit_command]
    (
        const std::string& base,
        std::function<position (const selection&)> get_position,
        std::function<position (VirtualView*, const position&)> modify_position
    )
    {
        add_edit_command
        (
            "doc.move-" + base,
            [get_view, get_position, modify_position]()
            {
                auto* view = get_view();
                if(view == nullptr) { return; }
                auto doc = view->doc;
                if(doc == nullptr) { return; }

                for(auto& sel: view->cursors)
                {
                    if(sel.is_selection())
                    {
                        const auto p = get_position(sel);
                        sel = {p, p};
                        view->scroll_to_cursor(p);
                    }
                    else
                    {
                        const auto np = modify_position(view, sel.b);
                        sel = {np, np};
                        view->scroll_to_cursor(np);
                    }
                }
                view->merge_all_cursors();
            }
        );
        add_edit_command
        (
            "doc.select-" + base,
            [get_view, get_position, modify_position]()
            {
                auto* view = get_view();
                if(view == nullptr) { return; }
                auto doc = view->doc;
                if(doc == nullptr) { return; }

                for(auto& sel: view->cursors)
                {
                    sel.b = modify_position(view, sel.b);
                    view->scroll_to_cursor(sel.b);
                }
                view->merge_all_cursors();
            }
        );
    };

    auto get_right_selection = [](const selection& s) -> position
    {
        return s.sorted().b;
    };

    auto get_left_selection = [](const selection& s) -> position
    {
        return s.sorted().a;
    };

    
    add_complex_command("right-char", get_right_selection, [](VirtualView* view, const position& p) -> position{
        return next_char(view->doc, p);
    });
    add_complex_command("left-char", get_left_selection, [](VirtualView* view, const position& p) -> position{
        return previous_char(view->doc, p);
    });
    add_complex_command("prev-block", get_left_selection, [](VirtualView* view, const position& p) -> position{
        return previous_block_start(view->doc, p);
    });
    add_complex_command("next-block", get_right_selection, [](VirtualView* view, const position& p) -> position{
        return next_block_end(view->doc, p);
    });
    add_complex_command("prev-line", get_left_selection, [](VirtualView* view, const position& p) -> position{
        const auto changed = change_line(view, p, -1);
        if(changed == p)
        {
            return start_of_line(p);
        }
        else
        {
            return changed;
        }
    });
    add_complex_command("next-line", get_right_selection, [](VirtualView* view, const position& p) -> position{
        const auto changed = change_line(view, p, 1);
        if(changed == p)
        {
            return end_of_line(p);
        }
        else
        {
            return changed;
        }
    });

    const auto add_word_commands =
    [
        &add_complex_command,
        &get_right_selection,
        &get_left_selection
    ]
    (
        const std::string& base,
        const is_word_fun& is_word_char
    )
    {
        add_complex_command("right-word"+base, get_right_selection, [is_word_char](VirtualView* view, const position& p) -> position{
            return next_word_end(view->doc, p, is_word_char);
        });
        add_complex_command("left-word"+base, get_left_selection, [is_word_char](VirtualView* view, const position& p) -> position{
            return previous_word_start(view->doc, p, is_word_char);
        });
        add_complex_command("word-start"+base, get_right_selection, [is_word_char](VirtualView* view, const position& p) -> position{
            return start_of_word(view->doc, p, is_word_char);
        });
        add_complex_command("word-end"+base, get_left_selection, [is_word_char](VirtualView* view, const position& p) -> position{
            return end_of_word(view->doc, p, is_word_char);
        });
    };
    
    add_word_commands
    (
        "",
        [](char c) -> bool
        {
            return is_whitespace(c)==false;
        }
    );

    /*
        A WORD is always delimited by whitespace.
        A word is delimited by non-keyword characters, which are configurable.
        Whitespace characters aren't keywords, and usually other characters (like ()[],-) aren't, neither.
        Therefore, a word usually is smaller than a WORD; the word-navigation is more fine-grained.

        This "stuff" is not-so difficult!
        wwww  wwwww  ww www ww wwwwwwwww    " (key)words, delimiters are non-keywords: "-! and whitespace
        WWWW WWWWWWW WW WWWWWW WWWWWWWWWW   " WORDS, delimiters are whitespace only

        https://stackoverflow.com/a/22931259
    */
   // todo(Gustav): small word doesn't quite work yet
    add_word_commands
    (
        "-special",
        [](char c) -> bool
        {
            constexpr std::string_view non_word_chars = " \t\n/\\()\"':,.;<>~!@#$%^&*|+=[]{}`?-";
            return non_word_chars.find(c) != std::string::npos;
        }
    );
    add_word_commands
    (
        "-abc",
        [](char c) -> bool
        {
            constexpr std::string_view abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
            return abc.find(c) == std::string::npos;
        }
    );

    // todo(Gustav): handle whitespace, home to start-of-line or home to first non-whitespace
    add_complex_command("home", get_left_selection, [](VirtualView*, const position& p) -> position{
        return start_of_line(p);
    });
    add_complex_command("end", get_right_selection, [](VirtualView*, const position& p) -> position{
        return end_of_line(p);
    });

    add_complex_command("doc-start", get_left_selection, [](VirtualView*, const position&) -> position{
        return start_of_doc();
    });
    add_complex_command("doc-end", get_right_selection, [](VirtualView* view, const position&) -> position{
        return end_of_doc(view->doc);
    });
}
