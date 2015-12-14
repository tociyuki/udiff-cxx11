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
        : oldfile (a), newfile (b), context (c),
          sfile (), efile (), sstuff (), estuff (),
          sdel (), edel (), sins (), eins () {}

    void decoration_none ();
    void ansi_color ();
    void print (std::vector<diff_type> const& change);
};
