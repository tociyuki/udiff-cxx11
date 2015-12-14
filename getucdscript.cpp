#include <cstdint>
#include "getucdscript.hpp"

struct ucdscript_type {
    uint32_t first, second;
    int script;
};

// Translated from Unicode UCD 7.0 Scripts.txt
static ucdscript_type UCDSCRIPT[] = {
    {0x000000UL, 0x000041UL,   1}, {0x000041UL, 0x00005bUL,   2},
    {0x00005bUL, 0x000061UL,   1}, {0x000061UL, 0x00007bUL,   2},
    {0x00007bUL, 0x0000aaUL,   1}, {0x0000aaUL, 0x0000abUL,   2},
    {0x0000abUL, 0x0000baUL,   1}, {0x0000baUL, 0x0000bbUL,   2},
    {0x0000bbUL, 0x0000c0UL,   1}, {0x0000c0UL, 0x0000d7UL,   2},
    {0x0000d7UL, 0x0000d8UL,   1}, {0x0000d8UL, 0x0000f7UL,   2},
    {0x0000f7UL, 0x0000f8UL,   1}, {0x0000f8UL, 0x0002b9UL,   2},
    {0x0002b9UL, 0x0002e0UL,   1}, {0x0002e0UL, 0x0002e5UL,   2},
    {0x0002e5UL, 0x0002eaUL,   1}, {0x0002eaUL, 0x0002ecUL,  35},
    {0x0002ecUL, 0x000300UL,   1}, {0x000300UL, 0x000370UL,  41},
    {0x000370UL, 0x000374UL,   3}, {0x000374UL, 0x000375UL,   1},
    {0x000375UL, 0x00037eUL,   3}, {0x00037eUL, 0x00037fUL,   1},
    {0x00037fUL, 0x000385UL,   3}, {0x000385UL, 0x000386UL,   1},
    {0x000386UL, 0x000387UL,   3}, {0x000387UL, 0x000388UL,   1},
    {0x000388UL, 0x0003e2UL,   3}, {0x0003e2UL, 0x0003f0UL,  55},
    {0x0003f0UL, 0x000400UL,   3}, {0x000400UL, 0x000485UL,   4},
    {0x000485UL, 0x000487UL,  41}, {0x000487UL, 0x000530UL,   4},
    {0x000531UL, 0x000588UL,   5}, {0x000589UL, 0x00058aUL,   1},
    {0x00058aUL, 0x000590UL,   5}, {0x000591UL, 0x0005f5UL,   6},
    {0x000600UL, 0x000605UL,   7}, {0x000605UL, 0x000606UL,   1},
    {0x000606UL, 0x00060cUL,   7}, {0x00060cUL, 0x00060dUL,   1},
    {0x00060dUL, 0x00061bUL,   7}, {0x00061bUL, 0x00061dUL,   1},
    {0x00061eUL, 0x00061fUL,   7}, {0x00061fUL, 0x000620UL,   1},
    {0x000620UL, 0x000640UL,   7}, {0x000640UL, 0x000641UL,   1},
    {0x000641UL, 0x00064bUL,   7}, {0x00064bUL, 0x000656UL,  41},
    {0x000656UL, 0x000660UL,   7}, {0x000660UL, 0x00066aUL,   1},
    {0x00066aUL, 0x000670UL,   7}, {0x000670UL, 0x000671UL,  41},
    {0x000671UL, 0x0006ddUL,   7}, {0x0006ddUL, 0x0006deUL,   1},
    {0x0006deUL, 0x000700UL,   7}, {0x000700UL, 0x000750UL,   8},
    {0x000750UL, 0x000780UL,   7}, {0x000780UL, 0x0007b2UL,   9},
    {0x0007c0UL, 0x0007fbUL,  66}, {0x000800UL, 0x00083fUL,  82},
    {0x000840UL, 0x00085fUL,  95}, {0x0008a0UL, 0x000900UL,   7},
    {0x000900UL, 0x000951UL,  10}, {0x000951UL, 0x000953UL,  41},
    {0x000953UL, 0x000964UL,  10}, {0x000964UL, 0x000966UL,   1},
    {0x000966UL, 0x000980UL,  10}, {0x000980UL, 0x0009fcUL,  11},
    {0x000a01UL, 0x000a76UL,  12}, {0x000a81UL, 0x000af2UL,  13},
    {0x000b01UL, 0x000b78UL,  14}, {0x000b82UL, 0x000bfbUL,  15},
    {0x000c00UL, 0x000c80UL,  16}, {0x000c81UL, 0x000cf3UL,  17},
    {0x000d01UL, 0x000d80UL,  18}, {0x000d82UL, 0x000df5UL,  19},
    {0x000e01UL, 0x000e3bUL,  20}, {0x000e3fUL, 0x000e40UL,   1},
    {0x000e40UL, 0x000e5cUL,  20}, {0x000e81UL, 0x000ee0UL,  21},
    {0x000f00UL, 0x000fd5UL,  22}, {0x000fd5UL, 0x000fd9UL,   1},
    {0x000fd9UL, 0x000fdbUL,  22}, {0x001000UL, 0x0010a0UL,  23},
    {0x0010a0UL, 0x0010fbUL,  24}, {0x0010fbUL, 0x0010fcUL,   1},
    {0x0010fcUL, 0x001100UL,  24}, {0x001100UL, 0x001200UL,  25},
    {0x001200UL, 0x00139aUL,  26}, {0x0013a0UL, 0x0013f5UL,  27},
    {0x001400UL, 0x001680UL,  28}, {0x001680UL, 0x00169dUL,  29},
    {0x0016a0UL, 0x0016ebUL,  30}, {0x0016ebUL, 0x0016eeUL,   1},
    {0x0016eeUL, 0x0016f9UL,  30}, {0x001700UL, 0x001715UL,  42},
    {0x001720UL, 0x001735UL,  43}, {0x001735UL, 0x001737UL,   1},
    {0x001740UL, 0x001754UL,  44}, {0x001760UL, 0x001774UL,  45},
    {0x001780UL, 0x0017faUL,  31}, {0x001800UL, 0x001802UL,  32},
    {0x001802UL, 0x001804UL,   1}, {0x001804UL, 0x001805UL,  32},
    {0x001805UL, 0x001806UL,   1}, {0x001806UL, 0x0018abUL,  32},
    {0x0018b0UL, 0x0018f6UL,  28}, {0x001900UL, 0x001950UL,  46},
    {0x001950UL, 0x001975UL,  47}, {0x001980UL, 0x0019e0UL,  56},
    {0x0019e0UL, 0x001a00UL,  31}, {0x001a00UL, 0x001a20UL,  54},
    {0x001a20UL, 0x001aaeUL,  78}, {0x001ab0UL, 0x001abfUL,  41},
    {0x001b00UL, 0x001b7dUL,  62}, {0x001b80UL, 0x001bc0UL,  67},
    {0x001bc0UL, 0x001c00UL,  93}, {0x001c00UL, 0x001c50UL,  68},
    {0x001c50UL, 0x001c80UL,  69}, {0x001cc0UL, 0x001cc8UL,  67},
    {0x001cd0UL, 0x001cd3UL,  41}, {0x001cd3UL, 0x001cd4UL,   1},
    {0x001cd4UL, 0x001ce1UL,  41}, {0x001ce1UL, 0x001ce2UL,   1},
    {0x001ce2UL, 0x001ce9UL,  41}, {0x001ce9UL, 0x001cedUL,   1},
    {0x001cedUL, 0x001ceeUL,  41}, {0x001ceeUL, 0x001cf4UL,   1},
    {0x001cf4UL, 0x001cf5UL,  41}, {0x001cf5UL, 0x001cf7UL,   1},
    {0x001cf8UL, 0x001cfaUL,  41}, {0x001d00UL, 0x001d26UL,   2},
    {0x001d26UL, 0x001d2bUL,   3}, {0x001d2bUL, 0x001d2cUL,   4},
    {0x001d2cUL, 0x001d5dUL,   2}, {0x001d5dUL, 0x001d62UL,   3},
    {0x001d62UL, 0x001d66UL,   2}, {0x001d66UL, 0x001d6bUL,   3},
    {0x001d6bUL, 0x001d78UL,   2}, {0x001d78UL, 0x001d79UL,   4},
    {0x001d79UL, 0x001dbfUL,   2}, {0x001dbfUL, 0x001dc0UL,   3},
    {0x001dc0UL, 0x001e00UL,  41}, {0x001e00UL, 0x001f00UL,   2},
    {0x001f00UL, 0x001fffUL,   3}, {0x002000UL, 0x00200cUL,   1},
    {0x00200cUL, 0x00200eUL,  41}, {0x00200eUL, 0x002071UL,   1},
    {0x002071UL, 0x002072UL,   2}, {0x002074UL, 0x00207fUL,   1},
    {0x00207fUL, 0x002080UL,   2}, {0x002080UL, 0x00208fUL,   1},
    {0x002090UL, 0x00209dUL,   2}, {0x0020a0UL, 0x0020beUL,   1},
    {0x0020d0UL, 0x0020f1UL,  41}, {0x002100UL, 0x002126UL,   1},
    {0x002126UL, 0x002127UL,   3}, {0x002127UL, 0x00212aUL,   1},
    {0x00212aUL, 0x00212cUL,   2}, {0x00212cUL, 0x002132UL,   1},
    {0x002132UL, 0x002133UL,   2}, {0x002133UL, 0x00214eUL,   1},
    {0x00214eUL, 0x00214fUL,   2}, {0x00214fUL, 0x002160UL,   1},
    {0x002160UL, 0x002189UL,   2}, {0x002189UL, 0x002800UL,   1},
    {0x002800UL, 0x002900UL,  53}, {0x002900UL, 0x002bd2UL,   1},
    {0x002c00UL, 0x002c5fUL,  57}, {0x002c60UL, 0x002c80UL,   2},
    {0x002c80UL, 0x002d00UL,  55}, {0x002d00UL, 0x002d2eUL,  24},
    {0x002d30UL, 0x002d80UL,  58}, {0x002d80UL, 0x002ddfUL,  26},
    {0x002de0UL, 0x002e00UL,   4}, {0x002e00UL, 0x002e43UL,   1},
    {0x002e80UL, 0x002fd6UL,  36}, {0x002ff0UL, 0x003005UL,   1},
    {0x003005UL, 0x003006UL,  36}, {0x003006UL, 0x003007UL,   1},
    {0x003007UL, 0x003008UL,  36}, {0x003008UL, 0x003021UL,   1},
    {0x003021UL, 0x00302aUL,  36}, {0x00302aUL, 0x00302eUL,  41},
    {0x00302eUL, 0x003030UL,  25}, {0x003030UL, 0x003038UL,   1},
    {0x003038UL, 0x00303cUL,  36}, {0x00303cUL, 0x003040UL,   1},
    {0x003041UL, 0x003097UL,  33}, {0x003099UL, 0x00309bUL,  41},
    {0x00309bUL, 0x00309dUL,   1}, {0x00309dUL, 0x0030a0UL,  33},
    {0x0030a0UL, 0x0030a1UL,   1}, {0x0030a1UL, 0x0030fbUL,  34},
    {0x0030fbUL, 0x0030fdUL,   1}, {0x0030fdUL, 0x003100UL,  34},
    {0x003105UL, 0x00312eUL,  35}, {0x003131UL, 0x00318fUL,  25},
    {0x003190UL, 0x0031a0UL,   1}, {0x0031a0UL, 0x0031bbUL,  35},
    {0x0031c0UL, 0x0031e4UL,   1}, {0x0031f0UL, 0x003200UL,  34},
    {0x003200UL, 0x00321fUL,  25}, {0x003220UL, 0x003260UL,   1},
    {0x003260UL, 0x00327fUL,  25}, {0x00327fUL, 0x0032d0UL,   1},
    {0x0032d0UL, 0x003358UL,  34}, {0x003358UL, 0x003400UL,   1},
    {0x003400UL, 0x004db6UL,  36}, {0x004dc0UL, 0x004e00UL,   1},
    {0x004e00UL, 0x009fcdUL,  36}, {0x00a000UL, 0x00a4c7UL,  37},
    {0x00a4d0UL, 0x00a500UL,  83}, {0x00a500UL, 0x00a62cUL,  70},
    {0x00a640UL, 0x00a6a0UL,   4}, {0x00a6a0UL, 0x00a6f8UL,  84},
    {0x00a700UL, 0x00a722UL,   1}, {0x00a722UL, 0x00a788UL,   2},
    {0x00a788UL, 0x00a78bUL,   1}, {0x00a78bUL, 0x00a800UL,   2},
    {0x00a800UL, 0x00a82cUL,  59}, {0x00a830UL, 0x00a83aUL,   1},
    {0x00a840UL, 0x00a878UL,  65}, {0x00a880UL, 0x00a8daUL,  71},
    {0x00a8e0UL, 0x00a8fcUL,  10}, {0x00a900UL, 0x00a92eUL,  72},
    {0x00a92eUL, 0x00a92fUL,   1}, {0x00a92fUL, 0x00a930UL,  72},
    {0x00a930UL, 0x00a960UL,  73}, {0x00a960UL, 0x00a97dUL,  25},
    {0x00a980UL, 0x00a9ceUL,  85}, {0x00a9cfUL, 0x00a9d0UL,   1},
    {0x00a9d0UL, 0x00a9e0UL,  85}, {0x00a9e0UL, 0x00a9ffUL,  23},
    {0x00aa00UL, 0x00aa60UL,  77}, {0x00aa60UL, 0x00aa80UL,  23},
    {0x00aa80UL, 0x00aae0UL,  79}, {0x00aae0UL, 0x00aaf7UL,  86},
    {0x00ab01UL, 0x00ab2fUL,  26}, {0x00ab30UL, 0x00ab5bUL,   2},
    {0x00ab5bUL, 0x00ab5cUL,   1}, {0x00ab5cUL, 0x00ab65UL,   2},
    {0x00ab65UL, 0x00ab66UL,   3}, {0x00abc0UL, 0x00abfaUL,  86},
    {0x00ac00UL, 0x00d7fcUL,  25}, {0x00f900UL, 0x00fadaUL,  36},
    {0x00fb00UL, 0x00fb07UL,   2}, {0x00fb13UL, 0x00fb18UL,   5},
    {0x00fb1dUL, 0x00fb50UL,   6}, {0x00fb50UL, 0x00fd3eUL,   7},
    {0x00fd3eUL, 0x00fd40UL,   1}, {0x00fd50UL, 0x00fdfeUL,   7},
    {0x00fe00UL, 0x00fe10UL,  41}, {0x00fe10UL, 0x00fe1aUL,   1},
    {0x00fe20UL, 0x00fe2eUL,  41}, {0x00fe30UL, 0x00fe6cUL,   1},
    {0x00fe70UL, 0x00fefdUL,   7}, {0x00feffUL, 0x00ff21UL,   1},
    {0x00ff21UL, 0x00ff3bUL,   2}, {0x00ff3bUL, 0x00ff41UL,   1},
    {0x00ff41UL, 0x00ff5bUL,   2}, {0x00ff5bUL, 0x00ff66UL,   1},
    {0x00ff66UL, 0x00ff70UL,  34}, {0x00ff70UL, 0x00ff71UL,   1},
    {0x00ff71UL, 0x00ff9eUL,  34}, {0x00ff9eUL, 0x00ffa0UL,   1},
    {0x00ffa0UL, 0x00ffddUL,  25}, {0x00ffe0UL, 0x00fffeUL,   1},
    {0x010000UL, 0x0100fbUL,  48}, {0x010100UL, 0x010140UL,   1},
    {0x010140UL, 0x01018dUL,   3}, {0x010190UL, 0x01019cUL,   1},
    {0x0101a0UL, 0x0101a1UL,   3}, {0x0101d0UL, 0x0101fdUL,   1},
    {0x0101fdUL, 0x0101feUL,  41}, {0x010280UL, 0x01029dUL,  74},
    {0x0102a0UL, 0x0102d1UL,  75}, {0x0102e0UL, 0x0102e1UL,  41},
    {0x0102e1UL, 0x0102fcUL,   1}, {0x010300UL, 0x010324UL,  38},
    {0x010330UL, 0x01034bUL,  39}, {0x010350UL, 0x01037bUL, 120},
    {0x010380UL, 0x0103a0UL,  49}, {0x0103a0UL, 0x0103d6UL,  60},
    {0x010400UL, 0x010450UL,  40}, {0x010450UL, 0x010480UL,  50},
    {0x010480UL, 0x0104aaUL,  51}, {0x010500UL, 0x010528UL, 106},
    {0x010530UL, 0x010570UL, 103}, {0x010600UL, 0x010768UL, 110},
    {0x010800UL, 0x010840UL,  52}, {0x010840UL, 0x010860UL,  87},
    {0x010860UL, 0x010880UL, 118}, {0x010880UL, 0x0108b0UL, 117},
    {0x010900UL, 0x010920UL,  64}, {0x010920UL, 0x010940UL,  76},
    {0x010980UL, 0x0109a0UL,  98}, {0x0109a0UL, 0x0109c0UL,  97},
    {0x010a00UL, 0x010a59UL,  61}, {0x010a60UL, 0x010a80UL,  88},
    {0x010a80UL, 0x010aa0UL, 116}, {0x010ac0UL, 0x010af7UL, 112},
    {0x010b00UL, 0x010b40UL,  80}, {0x010b40UL, 0x010b60UL,  89},
    {0x010b60UL, 0x010b80UL,  90}, {0x010b80UL, 0x010bb0UL, 121},
    {0x010c00UL, 0x010c49UL,  91}, {0x010e60UL, 0x010e7fUL,   7},
    {0x011000UL, 0x011080UL,  94}, {0x011080UL, 0x0110c2UL,  92},
    {0x0110d0UL, 0x0110faUL, 101}, {0x011100UL, 0x011144UL,  96},
    {0x011150UL, 0x011177UL, 111}, {0x011180UL, 0x0111dbUL, 100},
    {0x0111e1UL, 0x0111f5UL,  19}, {0x011200UL, 0x01123eUL, 109},
    {0x0112b0UL, 0x0112faUL, 123}, {0x011301UL, 0x011375UL, 107},
    {0x011480UL, 0x0114daUL, 124}, {0x011580UL, 0x0115caUL, 122},
    {0x011600UL, 0x01165aUL, 114}, {0x011680UL, 0x0116caUL, 102},
    {0x0118a0UL, 0x011900UL, 125}, {0x011ac0UL, 0x011af9UL, 119},
    {0x012000UL, 0x012475UL,  63}, {0x013000UL, 0x01342fUL,  81},
    {0x016800UL, 0x016a39UL,  84}, {0x016a40UL, 0x016a70UL, 115},
    {0x016ad0UL, 0x016af6UL, 104}, {0x016b00UL, 0x016b90UL, 108},
    {0x016f00UL, 0x016fa0UL,  99}, {0x01b000UL, 0x01b001UL,  34},
    {0x01b001UL, 0x01b002UL,  33}, {0x01bc00UL, 0x01bca0UL, 105},
    {0x01bca0UL, 0x01d167UL,   1}, {0x01d167UL, 0x01d16aUL,  41},
    {0x01d16aUL, 0x01d17bUL,   1}, {0x01d17bUL, 0x01d183UL,  41},
    {0x01d183UL, 0x01d185UL,   1}, {0x01d185UL, 0x01d18cUL,  41},
    {0x01d18cUL, 0x01d1aaUL,   1}, {0x01d1aaUL, 0x01d1aeUL,  41},
    {0x01d1aeUL, 0x01d1deUL,   1}, {0x01d200UL, 0x01d246UL,   3},
    {0x01d300UL, 0x01d800UL,   1}, {0x01e800UL, 0x01e8d7UL, 113},
    {0x01ee00UL, 0x01eef2UL,   7}, {0x01f000UL, 0x01f200UL,   1},
    {0x01f200UL, 0x01f201UL,  33}, {0x01f201UL, 0x01f8aeUL,   1},
    {0x020000UL, 0x02fa1eUL,  36}, {0x0e0001UL, 0x0e0080UL,   1},
    {0x0e0100UL, 0x0e01f0UL,  41},
};

int
getucdscript (uint32_t const c)
{
    int const n = sizeof (UCDSCRIPT) / sizeof (UCDSCRIPT[0]);
    if (c < UCDSCRIPT[0].first || UCDSCRIPT[n - 1].second <= c)
        return 0;
    int b = 0;
    int d = n;
    while (b < d) {
        int const u = b + (d - b) / 2;
        int const x = UCDSCRIPT[u].first;
        if (c < x)
            d = u;
        else if (c > x)
            b = u + 1;
        else
            return UCDSCRIPT[u].script;
    }
    return (b > 0 && UCDSCRIPT[b - 1].first <= c && c < UCDSCRIPT[b - 1].second)
        ? UCDSCRIPT[b - 1].script
        : 0;
}
