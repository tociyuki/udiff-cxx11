#include <string>
#include <vector>
#include <utility>
#include <cstdint>
#include <stdexcept>
#include "diff.hpp"

// subtext_type as a slice of text_type for Hirschberg's method.
//
//      {token_type token in text[i] for first <= i and i < second}
//
struct subtext_type {
    text_type& text;
    int first;
    int second;

    subtext_type (text_type& a, int const b, int const c)
        : text (a), first (b), second (c) {}
    bool empty () const { return first >= second; }
    int size () const { return second - first; }

    // find reverse direction from second - 1 to first.
    // returns index of found or first - 1 on not found.
    int rfind (token_type const& token)
    {
        for (int i = second; first < i; --i)
            if (text[i - 1].equal (token))
                return i - 1;
        return first - 1;
    }
};

// wu_midsnake function fill snake_type.
struct wu_midsnake_type {
    int x;
    int y;
    int u;
    int v;

    wu_midsnake_type () : x (-1), y (-1), u (-1), v (-1) {}
    wu_midsnake_type (wu_midsnake_type const&) = default;
    wu_midsnake_type& operator= (wu_midsnake_type const&) = default;
    int find (subtext_type& a, subtext_type& b);

private:
    bool find_down (subtext_type& a, subtext_type& b,
        std::vector<int>& fp, std::vector<int>& rp, int const pb, int& d);
    bool find_up (subtext_type& a, subtext_type& b,
        std::vector<int>& fp, std::vector<int>& rp, int const pb, int& d);
    int distance_down (int const k, int const delta, int const pb, bool const side);
    int distance_up (int const kdelta, int const delta, int const pb, bool const side);

    // safe fp[-n] and rp[-n] access.
    int
    pvec (std::vector<int> const& p, int const x)
    {
        int const y = x < 0 ? p.size () + x : x;
        return p[y];
    }

    // safe fp[-n] = a and rp[-n] = a.
    void
    pvec_set (std::vector<int>& p, int const x, int const a)
    {
        int const y = x < 0 ? p.size () + x : x;
        p[y] = a;
    }
};

// bidirectional differences between two texts implementation.
struct diffwu_impl {
    std::vector<diff_type> trunk;
    diffwu_impl () : trunk () {}
    void compute_diff_subtext (subtext_type& a, subtext_type& b);
    void compute_diff_core (subtext_type& a, subtext_type& b);
    void compute_diff_single_b (subtext_type& a, subtext_type& b);
    void compute_diff_single_a (subtext_type& a, subtext_type& b);
    void put (operation_type const operation, subtext_type& a);
    void put (operation_type const operation, subtext_type& a, int idx);
};

// start Hirschberg-Wu computation.
void
diffwu_type::compute_diff (text_type& atext, text_type& btext, std::vector<diff_type>& change)
{
    subtext_type a (atext, 0, atext.size ());
    subtext_type b (btext, 0, btext.size ());
    diffwu_impl impl;
    impl.compute_diff_subtext (a, b);
    std::swap (change, impl.trunk);
}

// trim common prefix and suffix for speed.
void
diffwu_impl::compute_diff_subtext (subtext_type& a, subtext_type& b)
{
    int af = a.first;
    int bf = b.first;
    int as = a.second;
    int bs = b.second;

    while (af < as && bf < bs && a.text[af].equal (b.text[bf]))
        ++af, ++bf;
    while (af < as && bf < bs && a.text[as - 1].equal (b.text[bs - 1]))
        --as, --bs;

    subtext_type prefix (a.text, a.first, af);
    subtext_type acore (a.text, af, as);
    subtext_type bcore (b.text, bf, bs);
    subtext_type suffix (a.text, as, a.second);

    put (EQUAL, prefix);
    compute_diff_core (acore, bcore);
    put (EQUAL, suffix);
}

// core part of Hirschberg-Wu method.
void
diffwu_impl::compute_diff_core (subtext_type& a, subtext_type& b)
{
    if (a.empty ())
        return put (INSERT, b);
    if (b.empty ())
        return put (DELETE, a);
    if (a.size () == 1)
        return compute_diff_single_a (a, b);
    if (b.size () == 1)
        return compute_diff_single_b (a, b);
    // bidirectional Wu's O(NP) computation finds
    // middle common part (x,y)-(u,v) and its edit distance d.
    wu_midsnake_type mid;
    int d = mid.find (a, b);
    if (d == 0) {
        put (EQUAL, a);
    }
    else if (d == 1 && mid.x < mid.y && mid.x == a.size () && mid.y == b.size ()) {
        put (EQUAL, a);
        put (INSERT, b, mid.y - 1);
    }
    else if (d == 1 && mid.x > mid.y && mid.x == a.size () && mid.y == b.size ()) {
        put (EQUAL, b);
        put (DELETE, a, a.first + mid.x - 1);
    }
    else if (d == 1 && mid.u - mid.x == a.size ()) {
        put (INSERT, b, b.first);
        put (EQUAL, a);
    }
    else if (d == 1 && mid.v - mid.y == b.size ()) {
        put (DELETE, a, a.first);
        put (EQUAL, b);
    }
    else if (mid.x == a.size () && mid.y == b.size () && a.size () <= b.size ()) {
        subtext_type acore (a.text, a.first, a.first + mid.x);
        subtext_type bcore (b.text, b.first, b.first + mid.y - 1);
        compute_diff_subtext (acore, bcore);
        put (INSERT, b, b.second - 1);
    }
    else if (mid.x == a.size () && mid.y == b.size ()) {
        subtext_type acore (a.text, a.first, a.first + mid.x - 1);
        subtext_type bcore (b.text, b.first, b.first + mid.y);
        compute_diff_subtext (acore, bcore);
        put (DELETE, a, a.second - 1);
    }
    else {
        // Hirschberg method
        subtext_type aleft (a.text, a.first, a.first + mid.x);
        subtext_type aright (a.text, a.first + mid.u, a.second);
        subtext_type mcore (a.text, a.first + mid.x, a.first + mid.u);
        subtext_type bleft (b.text, b.first, b.first + mid.y);
        subtext_type bright (b.text, b.first + mid.v, b.second);
        compute_diff_subtext (aleft, bleft);
        put (EQUAL, mcore);
        compute_diff_subtext (aright, bright);
    }
}

// the case of single token in subtext_type a.
void
diffwu_impl::compute_diff_single_a (subtext_type& a, subtext_type& b)
{
    int x = b.rfind (a.text[a.first]);
    if (x < b.first) {
        put (DELETE, a);
        put (INSERT, b);
    }
    else {
        subtext_type b0 (b.text, b.first, x);
        subtext_type b1 (b.text, x + 1, b.second);
        put (INSERT, b0);
        put (EQUAL, a);
        put (INSERT, b1);
    }
}

// the case of single token in subtext_type b.
void
diffwu_impl::compute_diff_single_b (subtext_type& a, subtext_type& b)
{
    int x = a.rfind (b.text[b.first]);
    if (x < a.first) {
        put (DELETE, a);
        put (INSERT, b);
    }
    else {
        subtext_type a0 (a.text, a.first, x);
        subtext_type a1 (a.text, x + 1, a.second);
        put (DELETE, a0);
        put (EQUAL, b);
        put (DELETE, a1);
    }
}

// put subtext with operation.
// DELETE before INSERT order.
void
diffwu_impl::put (operation_type const operation, subtext_type& a)
{
    if (a.empty ())
        return;
    for (int i = a.first; i < a.second; ++i) {
        trunk.emplace_back (operation);
        std::swap (trunk.back ().text, a.text[i].text);
    }
}

// put token in subtext at idx with operation.
void
diffwu_impl::put (operation_type const operation, subtext_type& a, int idx)
{
    trunk.emplace_back (operation);
    std::swap (trunk.back ().text, a.text[idx].text);
}

// find middle common part (snake) if possible.
// at returing, d is the edit distance, and snake is the set of middle snake.
//
// wu_midsnake is a bi-direction arrangement from Wu's O(NP) algorithm [2]
// with the same way of the Myers's bi-direction O(ND) variation [1].
//
// The original Hirschberg bi-direction method finds middle point in the
// editing graph two-dimension grid, and splits into the two grid at
// left and right side.  Myers arranged it with splitting into the three
// parts: left grid, single common path (middle snake), and right grid.
// The middle snake is found for the overlaped common snake in the
// left-top to middle set of paths and in the right-bottom to middle them.
//
// wu_midsnake also finds middle snake from both sides paths set.
// First for-loop in the while loop finds left-top to middle path.
// Second for-loop finds right-bottom to middle path.
// On O(NP) algorithm, a path may reachs left-top to right-bottom goal
// in the single iteration. So that in the first for-loop, we look for
// middle snake checking across the center line of the edit grid. 
// 
// [1]: E. W. Myers, ``An O(ND) Difference Algorithm and Its Variations'',
//  Algorithmica 1 (1986)
// [2]: S. Wu, U. Manber, G. Myers, W. Miller, ``An O(NP) Sequence Comparion Algorithm'',
//  Inf. Process. Lett. Volume 35 Issue6 (1990)
//
int
wu_midsnake_type::find (subtext_type& a, subtext_type& b)
{
    if (a.size () > b.size ()) {
        int d = find (b, a);
        std::swap (x, y);
        std::swap (u, v);
        return d;
    }
    int const m = a.size ();
    int const n = b.size ();
    std::vector<int> fp (m + n + 3, -1);
    std::vector<int> rp (m + n + 3, n + 1);
    int const delta = n - m;
    int pb = -1;
    while (delta + 4 * (pb + 1) - 2 <= m + n) {
        ++pb;
        int d;
        if (find_down (a, b, fp, rp, pb, d))
            return d;
        if (find_up (a, b, fp, rp, pb, d))
            return d;
    }
    throw std::runtime_error ("wu_midsnake_type: not found middle snake");
}

// find middle snake from left-top to middle.
bool
wu_midsnake_type::find_down (subtext_type& a, subtext_type& b,
    std::vector<int>& fp, std::vector<int>& rp, int const pb, int& d)
{
    int const m = a.size ();
    int const n = b.size ();
    int const delta = n - m;
    wu_midsnake_type mid;
    int middle = (n + 1) / 2;
    for (int k0 = -pb; k0 <= pb + delta; ++k0) {
        bool is_mid = false;
        int const k = k0 < delta ? k0 : pb + delta - (k0 - delta);
        y = std::max (pvec (fp, k - 1) + 1, pvec (fp, k + 1));
        x = y - k;
        v = y;
        u = x;
        while (u < m && v < n && a.text[a.first + u].equal (b.text[b.first + v])) {
            if (v == middle)
                is_mid = true;
            ++v;
            ++u;
        }
        pvec_set (fp, k, v);
        if (is_mid || (u <= m && v == middle))
            mid = *this;
        if (v == n || (pb > 0 && k >= -(pb - 1) && k <= pb - 1 + delta && v >= pvec (rp, k))) {
            d = distance_down (k, delta, pb, v == n);
            if (pb == 0 && d > 1 && u == m && v == n && mid.x >= 0)
                *this = mid;
            return true;
        }
    }
    return false;
}

// find middle snake from right-bottom to middle.
bool
wu_midsnake_type::find_up (subtext_type& a, subtext_type& b,
    std::vector<int>& fp, std::vector<int>& rp, int const pb, int& d)
{
    int const m = a.size ();
    int const n = b.size ();
    int const delta = n - m;
    for (int k0 = delta + pb; k0 >= -pb; --k0) {
        int const k = k0 > 0 ? k0 : -(pb + k0);
        y = std::min (pvec (rp, k - 1), pvec (rp, k + 1) - 1);
        x = y - k;
        v = y;
        u = x;
        while (x > 0 && x <= m && y > 0 && a.text[a.first + x - 1].equal (b.text[b.first + y - 1])) {
            --x;
            --y;
        }
        pvec_set (rp, k, y);
        if (y == 0 || (k >= -pb && k <= pb + delta && y <= pvec (fp, k))) {
            d = distance_up (k, delta, pb, y == 0);
            return true;
        }
    }
    return false;
}

// compute edit distance from left-top to middle.
int
wu_midsnake_type::distance_down (int const k, int const delta, int const pb, bool const side)
{
    int const fpb = k > delta ? pb + delta - k : pb;
    int const rpb = k < 0 ? pb - 1 + k : pb - 1;
    return side ? delta + 2 * pb : delta + 2 * (fpb + rpb);
}

// compute edit distance from right-bottom to middle.
int
wu_midsnake_type::distance_up (int const k, int const delta, int const pb, bool const side)
{
    int const fpb = k > delta ? pb + delta - k : pb;
    int const rpb = k < 0 ? pb + k : pb;
    return side ? delta + 2 * pb : delta + 2 * (fpb + rpb);
}
