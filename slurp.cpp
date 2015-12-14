#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include "slurp.hpp"

static bool slurp_content (std::istream& in, text_type& text);

// read entire lines from file or standard input ("-").
// return true on success to read and file not empty.
bool
slurp (std::string const& file, text_type& text)
{
    if (file == "-")
        slurp_content (std::cin, text);
    else {
        std::ifstream in;
        in.open (file.c_str ());
        if (! in.is_open ())
            return false;
        slurp_content (in, text);
        in.close ();
    }
    return ! text.empty ();
}

// read entire lines from std::istream.
// return true on success to read and text not empty.
static bool
slurp_content (std::istream& in, text_type& text)
{
    text.clear ();
    for (std::string str; std::getline (in, str); ) {
        text.emplace_back ();
        std::swap (text.back ().text, str);
        text.back().hashval = text.back ().murmurhash ();
    }
    return ! text.empty ();
}
