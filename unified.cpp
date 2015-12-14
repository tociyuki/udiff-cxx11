#include <string>
#include <vector>
#include <iostream>
#include "unified.hpp"

// working data for unified output.
struct linemark_type {
    bool mark;
    int ap;
    int bp;
    linemark_type () : mark (false), ap (0), bp (0) {}
};

// unified output
struct unified_impl {
    // file names
    std::string oldfile;
    std::string newfile;

    // context width (default 3)
    int context;  // assert (context >= 0);

    // output decoration
    std::string sfile;
    std::string efile;
    std::string sstuff;
    std::string estuff;
    std::string sdel;
    std::string edel;
    std::string sins;
    std::string eins;

    unified_impl (std::string const& a, std::string const& b, int const c)
        : oldfile (a), newfile (b), context (c),
          sfile (), efile (), sstuff (), estuff (),
          sdel (), edel (), sins (), eins () {}

    bool mark_lines (std::vector<diff_type> const& change, std::vector<linemark_type>& unify);
    void print_allhunk (std::vector<diff_type> const& change, std::vector<linemark_type> const& unify);
private:
    void
    print_hunk_stuff (std::vector<diff_type> const& change, std::vector<linemark_type> const& unify, int const hunk_begin, int const hunk_end);
    void
    print_hunk_content (std::vector<diff_type> const& change, int const hunk_begin, int const hunk_end);

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

// print change by unified style
void
unified_type::print (std::vector<diff_type> const& change)
{
    std::vector<linemark_type> linemark (change.size ());
    unified_impl impl (oldfile, newfile, context);
    impl.sfile = sfile;
    impl.efile = efile;
    impl.sstuff = sstuff;
    impl.estuff = estuff;
    impl.sdel = sdel;
    impl.edel = edel;
    impl.sins = sins;
    impl.eins = eins;
    if (impl.mark_lines (change, linemark))
        impl.print_allhunk (change, linemark);
}

// mark for print lines by unified style
bool
unified_impl::mark_lines (std::vector<diff_type> const& change, std::vector<linemark_type>& linemark)
{
    int a0 = 0, b0 = 0;
    int changed = false;
    for (int i = 0; i < change.size (); ++i) {
        if (INSERT != change[i].operation)
            ++a0;
        if (DELETE != change[i].operation)
            ++b0;
        linemark[i].ap = a0;
        linemark[i].bp = b0;
        if (EQUAL != change[i].operation) {
            changed = true;
            for (int j = i - context; j <= i + context; ++j) {
                if (0 <= j && j < linemark.size ())
                    linemark[j].mark = true;
            }
        }
    }
    return changed;
}

// print all hunk.
void
unified_impl::print_allhunk (std::vector<diff_type> const& change, std::vector<linemark_type> const& linemark)
{
    std::cout << sfile << "--- " << oldfile << efile << std::endl;
    std::cout << sfile << "+++ " << newfile << efile << std::endl;
    int hunk_begin = 0;
    bool hunk = false;
    for (int k = 0; k <= linemark.size (); ++k) {
        bool mark = k == linemark.size () ? false : linemark[k].mark;
        if (mark && ! hunk) {
            hunk_begin = k;
            hunk = true;
        }
        else if (hunk && ! mark) {
            print_hunk_stuff (change, linemark, hunk_begin, k);
            print_hunk_content (change, hunk_begin, k);
            hunk = false;
        }
    }
}

// print single hunk stuff
void
unified_impl::print_hunk_stuff (std::vector<diff_type> const& change, std::vector<linemark_type> const& linemark, int const hunk_begin, int const hunk_end)
{
    int a0 = -1, a1 = -1, b0 = -1, b1 = -1;
    for (int i = hunk_begin; i < hunk_end; ++i) {
        if (INSERT != change[i].operation) {
            if (a0 < 0)
                a0 = linemark[i].ap;
            a1 = linemark[i].ap;
        }
        if (DELETE != change[i].operation) {
            if (b0 < 0)
                b0 = linemark[i].bp;
            b1 = linemark[i].bp;
        }
    }
    std::cout << sstuff << "@@ -" << a0 << "," << a1 - a0 + 1
        << " +" << b0 << "," << b1 - b0 + 1 << " @@" << estuff << std::endl;
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
                    std::cout << sdel << "-" << change[i].text << edel << std::endl;
            else if (! insert_line.empty ())
                for (int i : insert_line)
                    std::cout << sins << "+" << change[i].text << eins << std::endl;
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
            std::cout << sdel << "-" << edel;
            line_top = false;
        }
        if (INSERT != change[j].operation) {
            if (change[j].text == "\n") {
                std::cout << std::endl;
                line_top = true;
            }
            else if (DELETE == change[j].operation) {
                std::cout << sdel << change[j].text << edel;
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
            std::cout << sins << "+" << eins;
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
                std::cout << sins << change[j].text << eins;
            }
        }
    }
}
