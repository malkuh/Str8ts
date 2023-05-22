#include "Straight.hpp"

using namespace std;

Straight::Straight(SMap* m, uint16_t* arg_start, uint16_t* arg_end, size_t arg_incr, int arg_starts_at) : map(m), start(arg_start), end(arg_end), incr(arg_incr), starts_at(arg_starts_at)
{
    length = (arg_end - arg_start)/arg_incr;
}

void Straight::check_range()
{
    uint16_t max_min = 511; // highest lower number
    uint16_t min_max = 511; // lowest higher number
    for (uint16_t* s = start; s < end; s += incr){
        max_min &= tz[*s];
        min_max &= lz[*s];
    }
    // add length
    max_min = tz[max_min>>(length-1)];
    min_max = lz[511&(min_max<<(length-1))];
    // take intersection
    range &= max_min & min_max;
    // restrict digits to range
    for (uint16_t* s = start; s < end; s += incr)
        *s &= range;
    // look up sure digits 
    sure |= sure_digits[length-1][range];
}

void Straight::sure_candidates(uint16_t all_sures)
{
    uint16_t others = all_sures & ~sure;
    for (uint16_t* s = start; s < end; s += incr)
        *s &= ~others;
}

void Straight::sure_singles()
{
    uint16_t once = 0;
    uint16_t twice = 0;
    uint16_t temp = 0;
    uint16_t set = 0;
    for (uint16_t* s = start; s < end; s += incr){
        temp = sure & *s;
        twice |= once & temp;
        once |= temp;
        if (singles[*s])
            set += *s;
    }
    uint16_t single_digits = once ^ twice;
    // look where the singles are and set that field to the single digit
    if (single_digits & ~set) {
        for (uint16_t* s = start; s < end; s += incr) {
            if (single_digits & (*s))
                *s = single_digits & (*s);
        }
    }
}

void Straight::stranded_digits()
{
    uint16_t impossible = 511;
    for (uint16_t* s = start; s < end; s += incr)
        impossible &= ~*s;
    // set bits in impossible correspond to impossible digits 
    if (impossible & range) {
        range &= p_range[length-1][range & ~impossible];
        sure |= sure_digits[length-1][range & ~impossible];
        // restrict digits to range
        for (uint16_t* s = start; s < end; s += incr)
            *s &= range;
    }
}

void Straight::naked_pairs()
{
    // there are only n numbers possible in n fields. making them sure candidates -> can be removed
    // this works also across straights in a row or column
    // for pairs
    for (uint16_t* s1 = start; s1 < end; s1 += incr) {
        if (set_bits[*s1] == 2) {
            for (uint16_t* s2 = s1+incr; s2 < end; s2 += incr) {
                if (*s1 == *s2)
                    sure |= *s1;
            }
        }
    }
}

void Straight::hidden_pairs()
{
    // there are only n same numbers sure in n fields. other candidates can be removed
    // this works also across straights in a row or column
    // for pairs
    for (uint16_t* s1 = start; s1 < end; s1 += incr) {
        if (set_bits[*s1 & sure] == 2) {
            for (uint16_t* s2 = s1+incr; s2 < end; s2 += incr) {
                if (*s1 & (sure == *s2) & sure) {
                    *s1 &= sure;
                    *s2 &= sure;
                }
            }
        }
    }
}

bool Straight::range_violation()
{
    uint16_t set = 0;
    for (uint16_t* s = start; s < end; s += incr){
        if (singles[*s])
            set |= *s;
    }
    uint16_t set_range = lz[set] & tz[set];
    if (set_bits[set_range] <= length)
        return false;
    else
        range_violation_verbose();
        return true;
}
