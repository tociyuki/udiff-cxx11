#include <string>
#include <deque>
#include "optlong.hpp"

optlong_type::optlong_type (int argc, char *argv[]) : field ()
{
    for (int i = 1; i < argc; ++i)
        field.emplace_back (argv[i]);
    argp = 1;
}

bool optlong_type::is_option ()
{
    if (field.empty () || field[0].size () < 2 || field[0][0] != '-')
        return false;
    if (field[0] == "-")
        return false;
    if (field[0] == "--") {
        field.pop_front ();
        return false;
    }
    return true;
}

bool optlong_type::getopt (std::string const name, char const single)
{
    if (field[0] == name) {
        field.pop_front ();
        return true;
    }
    else if (single && field[0][argp] == single) {
        if (++argp >= field[0].size ()) {
            field.pop_front ();
            argp = 1;
        }
        return true;
    }
    return false;
}

bool optlong_type::getopt (std::string const name, char const single, int& value)
{
    int i = 0;
    int p = argp;
    if (field[i].compare (0, name.size (), name) == 0) {
        p = name.size ();
    }
    else if (single && field[i][p] == single) {
        if (++p >= field[i].size ()) {
            ++i;
            p = 0;
        }
    }
    else
        return false;
    if (i < field.size () && p < field[i].size ()) {
        int x = 0;
        for (; p < field[i].size (); ++p) {
            if (field[i][p] < '0' || '9' < field[i][p] || x > 100000000L)
                return false;
            x = x * 10 + field[i][p] - '0';
        }
        value = x;
        if (i > 0)
            field.pop_front ();
        field.pop_front ();
        argp = 1;
        return true;
    }
    return false;
}
