#ifndef STRAIGHT
#define STRAIGHT

#include<iostream>
#include<inttypes.h>

#include "LUTs.hpp"

typedef uint16_t Candidates; // Bitmaps for each field.
                             // If n is a possible digit in field i,
                             // then 2^(n-1) & fields[i] is true.

struct SMap;

struct Straight
{
    // ptr to original SMap
    const SMap* map;
    
    // das sind pointer auf Array-Elemente in SMap's fields:
    uint16_t *start, *end;

    // increment is either 1 or 9 depending if it's a row or column straight
    const size_t incr = 1;

    // sure digits
    uint16_t sure = 0;

    // possible range
    uint16_t range = 511;

    // number of digits in the straight
    const char length;
    
    // Straight starts at position:
    const int starts_at;
    const int ends_at;

    Straight(SMap* m, uint16_t* arg_start, uint16_t* arg_end, size_t arg_incr, int arg_starts_at);
    Straight(SMap* m);

    // heuristics
    void check_range();
    void sure_candidates(uint16_t all_sures);
    void sure_singles();
    void stranded_digits();
    void naked_pairs();
    void hidden_pairs();

    bool range_violation();
    void range_violation_verbose() const;

};

#endif
