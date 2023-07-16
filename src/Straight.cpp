#include "Straight.hpp"
#include "verbose.hpp"

using namespace std;

Straight::Straight(
        SMap* m,
        Candidates* arg_start,
        Candidates* arg_end,
        size_t arg_incr,
        int arg_starts_at
    ) :
    map(m),
    start(arg_start),
    end(arg_end),
    incr(arg_incr),
    length((arg_end - arg_start)/arg_incr),
    starts_at(arg_starts_at),
    ends_at(starts_at + (int) (length * incr))
{
    // assert(incr == 1 || incr == 9);
}

void Straight::check_range()
{
    Candidates max_min = 511; // highest lower number
    Candidates min_max = 511; // lowest higher number
    for (auto s = start; s < end; s += incr){
        max_min &= tz[*s];
        min_max &= lz[*s];
    }
    // add length
    max_min = tz[max_min>>(length-1)];
    min_max = lz[511&(min_max<<(length-1))];
    // take intersection
    range &= max_min & min_max;
    // restrict digits to range
    for (auto s = start; s < end; s += incr)
        *s &= range;
    // look up sure digits 
    sure |= sure_digits[length-1][range];
}

void Straight::sure_candidates(Candidates all_sures)
{
    Candidates others = all_sures & ~sure;
    for (auto s = start; s < end; s += incr)
        *s &= ~others;
}

void Straight::sure_singles()
{
    Candidates once = 0;
    Candidates twice = 0;
    Candidates temp = 0;
    Candidates set = 0;
    for (auto s = start; s < end; s += incr){
        temp = sure & *s;
        twice |= once & temp;
        once |= temp;
        if (singles[*s])
            set += *s;
    }
    Candidates single_digits = once ^ twice;
    // look where the singles are and set that field to the single digit
    if (single_digits & ~set) {
        for (auto s = start; s < end; s += incr) {
            if (single_digits & (*s))
                *s = single_digits & (*s);
        }
    }
}

void Straight::stranded_digits()
{
    Candidates impossible = 511;
    for (auto s = start; s < end; s += incr)
        impossible &= ~*s;
    // set bits in impossible correspond to impossible digits 
    if (impossible & range) {
        range &= p_range[length-1][range & ~impossible];
        sure |= sure_digits[length-1][range & ~impossible];
        // restrict digits to range
        for (auto s = start; s < end; s += incr)
            *s &= range;
    }
}

void Straight::naked_pairs()
{
    // there are only n numbers possible in n fields. making them sure candidates -> can be removed
    // this works also across straights in a row or column
    // for pairs
    for (auto s1 = start; s1 < end; s1 += incr) {
        if (set_bits[*s1] == 2) {
            for (auto s2 = s1+incr; s2 < end; s2 += incr) {
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
    for (auto s1 = start; s1 < end; s1 += incr) {
        if (set_bits[*s1 & sure] == 2) {
            for (auto s2 = s1+incr; s2 < end; s2 += incr) {
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
    Candidates set = 0;
    for (auto s = start; s < end; s += incr){
        if (singles[*s])
            set |= *s;
    }
    Candidates set_range = lz[set] & tz[set];
    if (set_bits[set_range] <= length)
        return false;
    else
        range_violation_verbose();
        return true;
}

void Straight::range_violation_verbose() const {
    
    if (!Verbose::on) return;
    
    std::string straight = "";
    if ( incr == 1 ) {
        // row
        const int col = starts_at % 9;
        straight += Verbose::ROWS[starts_at/9];
        for ( int i = col; i < col + length; i += 1 ) {
            straight += Verbose::COLS[i];
        }
        
    } else {
        // col
        const int row = starts_at / 9;
        for ( int i = row; i < row + length; i += 1 ) {
            straight += Verbose::ROWS[i];
        }
        straight += Verbose::COLS[starts_at%9];
    }
    
    std::cout << "range violation in " << straight << ":\n";
}
