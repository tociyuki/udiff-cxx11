#include <string>
#include <vector>
#include <iostream>
#include "unified.hpp"

// unified output
struct unified_impl {
    unified_type& face;

    unified_impl (unified_type& a) : face (a) {}

    void print_allhunk (std::vector<diff_type> const& change);
private:
    void print_heading ();
    void print_hunk_stuff (int const a0, int const a1, int const b0, int const b1);
    void print_hunk_content (std::vector<diff_type> const& change, int const hunk_begin, int const hunk_end);

    void diff_word (std::vector<diff_type> const& change_coarse,
        std::vector<int> const& delete_line, std::vector<int> const& insert_line);
    void print_replace (std::vector<diff_type>const& change);
    void print_replace_delete (std::vector<diff_type>const& change);
    void print_replace_insert (std::vector<diff_type>const& change);
};

// clear output decoration
void
unified_type::decoration_none ()
{
    sfile.clear ();
    efile.clear ();
    sstuff.clear ();
    estuff.clear ();
    sdel.clear ();
    edel.clear ();
    sins.clear ();
    eins.clear ();
}

// set output color decoration for ANSI terminal escape sequence
void
unified_type::ansi_color ()
{
    // ANSI terminal escape sequence color code
    //  0 reset       1 bold          2 dim         4 underline
    // 30 darkblack  31 darkred      32 darkgreen  33 darkyellow
    // 34 darkblue   35 darkmagenta  36 darkcyan   37 darkwhite
    // 90 black      91 red          92 green      93 yellow
    // 94 blue       95 magenta      96 cyan       97 white
    sfile  = "\033[36m"; // darkcyan
    efile  = "\033[0m";
    sstuff = "\033[33m"; // darkyellow
    estuff = "\033[0m";
    sdel   = "\033[31m"; // darkred
    edel   = "\033[0m";
    sins   = "\033[32m"; // darkgreen
    eins   = "\033[0m";
}

// ignore_space_change for each line
void unified_type::ignore_space_change (text_type& a)
{
    for (int j = 0; j < a.size (); ++j)
        a[j].ignore_space_change ();
}

// print change by unified style
void
unified_type::print (std::vector<diff_type> const& change)
{
    unified_impl impl (*this);
    impl.print_allhunk (change);
}

// print all hunk.
void
unified_impl::print_allhunk (std::vector<diff_type> const& change)
{
    int a0 = 0, a1 = 0, b0 = 0, b1 = 0;
    int h0 = 0, h1 = 0;
    int nhunk = 0;
    int state = 1;
    for (int k = 0; k <= change.size (); ++k) {
        operation_type op = k == change.size () ? EQUAL : change[k].operation;
        if (EQUAL != op) {
            if (1 == state) {
                if (h0 < k - face.context)
                    h0 = k - face.context;
                a0 = -1, b0 = -1;
                if (k - h0 > 0) {
                    a0 = a1 - k + h0 + 1;
                    b0 = b1 - k + h0 + 1;
                }
                state = 2;
            }
            h1 = k + 1;
        }
        else if (2 == state && (k - h1 >= 2 * face.context || k == change.size ())) {
            h1 += face.context;
            if (h1 > change.size ())
                h1 = change.size ();
            if (++nhunk == 1)
                print_heading ();
            print_hunk_stuff (a0, a1 - k + h1, b0, b1 - k + h1);
            print_hunk_content (change, h0, h1);
            h0 = h1;
            state = 1;
        }
        if (INSERT != op) {
            ++a1;
            if (a0 < 0)
                a0 = a1;
        }
        if (DELETE != op) {
            ++b1;
            if (b0 < 0)
                b0 = b1;
        }
    }
}

// print file heading
void
unified_impl::print_heading ()
{
    std::cout << face.sfile << "--- " << face.oldfile << face.efile << std::endl;
    std::cout << face.sfile << "+++ " << face.newfile << face.efile << std::endl;
}

// print single hunk stuff
void
unified_impl::print_hunk_stuff (int const a0, int const a1, int const b0, int const b1)
{
    std::cout << face.sstuff << "@@ - ";
    if (a0 < 0)
        std::cout << a1 << ",0";
    else {
        std::cout << a0;
        if (a1 > a0)
            std::cout << "," << a1 - a0 + 1;
    }
    if (b0 < 0)
        std::cout << " +" << b1 << ",0";
    else {
        std::cout << " +" << b0;
        if (b1 > b0)
            std::cout << "," << b1 - b0 + 1;
    }
    std::cout << " @@" << face.estuff << std::endl;
}

// print single hunk content
void
unified_impl::print_hunk_content (std::vector<diff_type> const& change, int const hunk_begin, int const hunk_end)
{
    std::vector<int> delete_line;
    std::vector<int> insert_line;
    for (int j = hunk_begin; j <= hunk_end; ++j) {
        int const op = j == hunk_end ? EQUAL : change[j].operation;
        switch (op) {
        case DELETE:
            delete_line.push_back (j);
            break;
        case EQUAL:
            if (! delete_line.empty () && ! insert_line.empty ())
                diff_word (change, delete_line, insert_line);
            else if (! delete_line.empty ())
                for (int i : delete_line)
                    std::cout << face.sdel << "-" << change[i].text << face.edel << std::endl;
            else if (! insert_line.empty ())
                for (int i : insert_line)
                    std::cout << face.sins << "+" << change[i].text << face.eins << std::endl;
            delete_line.clear ();
            insert_line.clear ();
            if (j < hunk_end)
                std::cout << " " << change[j].text << std::endl;
            break;
        case INSERT:
            insert_line.push_back (j);
            break;
        }
    }
}

// word based diff for a change part.
void
unified_impl::diff_word (std::vector<diff_type> const& change_coarse,
    std::vector<int> const& delete_line, std::vector<int> const& insert_line)
{
    text_type aword;
    text_type bword;
    for (int i : delete_line) {
        token_type line (change_coarse[i].text, 0);
        line.split_word (aword);
    }
    for (int i : insert_line) {
        token_type line (change_coarse[i].text, 0);
        line.split_word (bword);
    }
    std::vector<diff_type> change_fine;
    diffwu_type diffwu;
    diffwu.compute_diff (aword, bword, change_fine);
    print_replace (change_fine);
}

// print a replace part with word based diff.
void
unified_impl::print_replace (std::vector<diff_type>const& change)
{
    print_replace_delete (change);
    print_replace_insert (change);
}

// print delete lines in the replace part.
void
unified_impl::print_replace_delete (std::vector<diff_type>const& change)
{
    bool line_top = true;
    for (int j = 0; j < change.size (); ++j) {
        if (line_top) {
            std::cout << face.sdel << "-" << face.edel;
            line_top = false;
        }
        if (INSERT != change[j].operation) {
            if (change[j].text == "\n") {
                std::cout << std::endl;
                line_top = true;
            }
            else if (DELETE == change[j].operation) {
                std::cout << face.sdel << change[j].text << face.edel;
            }
            else if (EQUAL == change[j].operation) {
                std::cout << change[j].text;
            }
        }
    }
}

// print insert lines in the replace part.
void
unified_impl::print_replace_insert (std::vector<diff_type>const& change)
{
    bool line_top = true;
    for (int j = 0; j < change.size (); ++j) {
        if (line_top) {
            std::cout << face.sins << "+" << face.eins;
            line_top = false;
        }
        if (DELETE != change[j].operation) {
            if (change[j].text == "\n") {
                std::cout << std::endl;
                line_top = true;
            }
            else if (EQUAL == change[j].operation) {
                std::cout << change[j].text;
            }
            else if (INSERT == change[j].operation) {
                std::cout << face.sins << change[j].text << face.eins;
            }
        }
    }
}
