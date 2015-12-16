#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "optlong.hpp"
#include "diff.hpp"
#include "slurp.hpp"
#include "unified.hpp"

void print_help (std::string const& program)
{
    std::cout
        << program << " option old new" << std::endl
        << "compute differences between two text files." << std::endl
        << "only unified output format is supported." << std::endl
        << "word-diff turns on at change blocks." << std::endl
        << std::endl
        << "options:" << std::endl
        << std::endl
        << " -b, --ignore-space-change  trim end line spaces." << std::endl
        << "     --color    colored." << std::endl
        << "     --nocolor  no colored." << std::endl
        << " -u, -U NUM, --unified[=NUM]" << std::endl
        << "                unified context number (default 3)." << std::endl
        << "     --help     print help." << std::endl
        << " -v, --version  print version." << std::endl;
    std::cout << std::endl;
    exit (EXIT_SUCCESS);
}

void print_version (std::string const& program)
{
    std::cout << program << " version 0.01" << std::endl;
    exit (EXIT_SUCCESS);
}

// differences between two files.
int
main (int argc, char *argv[])
{
    std::string progname (argv[0]);
    optlong_type arg (argc, argv);
    bool opt_ignore_space_change = false;
    bool word_color = isatty (STDOUT_FILENO) != 0;
    int context = 3;
    while (arg.is_option ()) {
        if (arg.getopt ("--ignore-space-chage", 'b')) {
            opt_ignore_space_change = true;
            continue;
        }
        if (arg.getopt ("--color", '\0')) {
            word_color = true;
            continue;
        }
        if (arg.getopt ("--nocolor", '\0') || arg.getopt ("--no-color", '\0')) {
            word_color = false;
            continue;
        }
        if (arg.getopt ("--unified", 'u'))
            continue;
        if (arg.getopt ("--unified=", 'U', context))
            continue;
        if (arg.getopt ("--help", '\0'))
            print_help (progname);
        if (arg.getopt ("--version", 'v'))
            print_version (progname);
        std::cerr << progname << ": unknown '" << arg.field[0] << "'." << std::endl;
        std::cerr << "please --help." << std::endl;
        exit (EXIT_FAILURE);
    }
    if (arg.field.size () < 2) {
        std::cerr << "usage: " << progname << " option old new" << std::endl;
        exit (EXIT_FAILURE);
    }
    if (arg.field[0] == "-" && arg.field[1] == "-") {
        std::cerr << progname << ": cannot diff from stdin to stdin." << std::endl;
        exit (EXIT_FAILURE);
    }

    unified_type unified (arg.field[0], arg.field[1], context);
    unified.opt_ignore_space_change = opt_ignore_space_change;
    if (word_color)
        unified.ansi_color ();

    text_type a;
    text_type b;
    if (! slurp (unified.oldfile, a)) {
        std::cerr << progname << ": cannot read '" << unified.oldfile << "'." << std::endl;
        exit (EXIT_FAILURE);
    }
    if (! slurp (unified.newfile, b)) {
        std::cerr << progname << ": cannot read '" << unified.newfile << "'." << std::endl;
        exit (EXIT_FAILURE);
    }
    if (unified.opt_ignore_space_change) {
        unified.ignore_space_change (a);
        unified.ignore_space_change (b);
    }
    std::vector<diff_type> change;
    diffwu_type diffwu;
    diffwu.compute_diff (a, b, change);
    unified.print (change);

    return EXIT_SUCCESS;
}
