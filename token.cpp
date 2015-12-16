#include <string>
#include <cstdint>
#include "diff.hpp"
#include "getucdscript.hpp"

// ignore space change from text to cook.
// spaces at end of line are trimmed.
// another spaces compare to be equal.
void
token_type::ignore_space_change ()
{
    cook.clear ();
    bool space = false;
    for (int i = 0; i < text.size (); ++i) {
        if (' ' == text[i])
            space = true;
        else {
            if (space)
                cook.push_back (' ');
            cook.push_back (text[i]);
            space = false;
        }
    }
    hashval = murmurhash ();
}

// split a line to a vector of word based tokens.
// the line must be chomped the last '\n' character.
// a sequence of spaces is also single token.
void
token_type::split_word (text_type& a)
{
    int first = 0;
    int last_script = 0;
    for (int i = 0; i <= text.size (); ) {
        // UTF-8 multibyte to code point uc.
        int u8skip = 1;
        uint32_t uc = i == text.size () ? 0 : ord (i, u8skip);
        // get UCD Script for Unicode-7.0
        // special treatments for few code points.
        int script = i == text.size () ? -4
            : '\t' == uc || ' ' == uc ? -3
            : '0' <= uc && uc <= '9' ? -2
            : uc < 0x20 || 0x7f == uc ? -1
            : '_' == uc ? 2
            : getucdscript (uc);
        if (script != last_script && first < i) {
            a.emplace_back ();
            a.back ().text.assign (text.begin () + first, text.begin () + i);
            a.back ().cook.assign (a.back ().text);
            a.back ().hashval = a.back ().murmurhash ();
            first = i;
        }
        last_script = script;
        i += u8skip;
    }
    a.emplace_back ();
    a.back ().text.assign ("\n");
    a.back ().cook.assign (a.back ().text);
    a.back ().hashval = a.back ().murmurhash ();
}

// 32bit hash function (little endian) of a line buf.substr (pos, len).
//
//   A. Appleby, ``MurmurHash'',
//   https://sites.google.com/site/murmurhash/
//
uint32_t
token_type::murmurhash () const
{
    static const uint32_t m = 0x5bd1e995UL;
    std::string::const_iterator s = cook.cbegin ();
    uint32_t n = cook.size ();
    uint32_t h = n * m;
    for (; n >= 4; n -= 4, s += 4) {
        uint32_t const u
            =  static_cast<uint8_t> (s[0])
            | (static_cast<uint8_t> (s[1]) <<  8)
            | (static_cast<uint8_t> (s[2]) << 16)
            | (static_cast<uint8_t> (s[3]) << 24);
        h += u;
        h *= m;
        h ^= h >> 16;
    }
    switch (n) {
    case 3:
        h += static_cast<uint8_t> (s[2]) << 16;
    case 2:
        h += static_cast<uint8_t> (s[1]) <<  8;
    case 1:
        h += static_cast<uint8_t> (s[0]);
        h *= m;
        h ^= h >> 16;
    }
    h *= m;
    h ^= h >> 10;
    h *= m;
    h ^= h >> 17;
    return h;
}

// peek one Unicode code point at position i.
// returns skip size of UTF-8 multibyte.
uint32_t
token_type::ord (int const i, int& u8skip) const
{
    uint32_t uc = static_cast<uint8_t> (text[i]);
    u8skip = (0xc0 == (0xe0 & uc) && i + 1 < text.size ()) ? 2
           : (0xe0 == (0xf0 & uc) && i + 2 < text.size ()) ? 3
           : (0xf0 == (0xf8 & uc) && i + 3 < text.size ()) ? 4
           : 1;
    if (2 == u8skip) {
        uc &= 0x1f;
        uc = (uc << 6) | (0x3f & static_cast<uint8_t> (text[i + 1]));
    }
    else if (3 == u8skip) {
        uc &= 0x0f;
        uc = (uc << 6) | (0x3f & static_cast<uint8_t> (text[i + 1]));
        uc = (uc << 6) | (0x3f & static_cast<uint8_t> (text[i + 2]));
    }
    else if (4 == u8skip) {
        uc &= 0x07;
        uc = (uc << 6) | (0x3f & static_cast<uint8_t> (text[i + 1]));
        uc = (uc << 6) | (0x3f & static_cast<uint8_t> (text[i + 2]));
        uc = (uc << 6) | (0x3f & static_cast<uint8_t> (text[i + 3]));
    }
    return uc;
}
