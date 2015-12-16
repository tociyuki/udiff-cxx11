#include <string>
#include <vector>
#include "diff.hpp"

// unified output
struct unified_type {
    // file names
    std::string oldfile;
    std::string newfile;

    // context width (default 3)
    int context;  // assert (context >= 0);

    // -b, --ignore-space-change
    bool opt_ignore_space_change;

    // output decoration
    std::string sfile;
    std::string efile;
    std::string sstuff;
    std::string estuff;
    std::string sdel;
    std::string edel;
    std::string sins;
    std::string eins;

    unified_type (std::string const& a, std::string const& b, int const c)
        : oldfile (a), newfile (b), context (c), opt_ignore_space_change (false),
          sfile (), efile (), sstuff (), estuff (),
          sdel (), edel (), sins (), eins () {}

    void ignore_space_change (text_type& a);
    void decoration_none ();
    void ansi_color ();
    void print (std::vector<diff_type> const& change);
};
