#pragma once
#include <string>
#include <deque>

class optlong_type {
    int argp;
public:
    std::deque<std::string> field;

    optlong_type (int argc, char *argv[]);
    bool is_option ();
    bool getopt (std::string const name, char const single);
    bool getopt (std::string const name, char const single, int& value);
};
