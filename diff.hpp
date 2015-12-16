#pragma once
#include <string>
#include <vector>
#include <utility>
#include <cstdint>

// diff_type is introduced by google-diff-match-patch library.

enum operation_type { DELETE = -1, EQUAL = 0, INSERT = +1 };

struct diff_type {
    operation_type operation;
    std::string text;

    diff_type () : operation (EQUAL), text () {}
    diff_type (operation_type const a) : operation (a), text () {}
    diff_type (operation_type const a, std::string const& b) : operation (a), text (b) {}
    diff_type (operation_type const a, std::string&& b) : operation (a), text (std::move (b)) {}
};

struct token_type;
typedef std::vector<token_type> text_type;

// unit of comparison for text is token_type.
struct token_type {
    std::string text;
    std::string cook;
    uint32_t hashval;

    token_type () : text (), cook (), hashval (0) {}
    token_type (std::string const& a, uint32_t b) : text (a), cook (a), hashval (b) {}
    void ignore_space_change ();
    void split_word (text_type& a);
    uint32_t murmurhash () const;
    uint32_t ord (int const i, int& u8skip) const;

    bool equal (token_type const& x) const
    {
        return hashval == x.hashval && cook == x.cook;
    }
};

// bidirectional differences between two texts interface.
struct diffwu_type {
    void compute_diff (text_type& atext, text_type& btext, std::vector<diff_type>& change);
};
