#include <string>
#include <fstream>
#include "diff.hpp"
#include "taptests.hpp"

int
main ()
{
    test::simple ts;
    std::fstream in;
    in.open ("diff-test.spec");
    if (in.good ()) {
        for (std::string line; std::getline (in, line); ) {
            std::size_t i = line.find (' ');
            std::string astr (line.begin (), line.begin () + i);
            std::string bstr (line.begin () + i + 1, line.end ());
            text_type a;
            text_type b;
            for (int c : astr) {
                a.emplace_back ();
                a.back ().text.push_back (c);
                a.back ().hashval = a.back ().murmurhash ();
            }
            for (int c : bstr) {
                b.emplace_back ();
                b.back ().text.push_back (c);
                b.back ().hashval = b.back ().murmurhash ();
            }
            std::vector<diff_type> change;
            diffwu_type wu;
            wu.compute_diff (a, b, change);
            std::string s;
            std::string t;
            for (auto u : change) {
                if (u.operation != INSERT)
                    s += u.text;
                if (u.operation != DELETE)
                    t += u.text;
            }
            ts.ok (astr == s && bstr == t, line);
        }
    }
    return ts.done_testing ();
}
