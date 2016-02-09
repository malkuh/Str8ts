#ifndef LUTS_H
#define LUTS_H

#include <inttypes.h>

// ranges contains numbers with a different number of set bits from 1 to 9
// beginning from the first bit.
extern uint16_t ranges[9];

// numbers contains all nine possible digits by themselves.
extern uint16_t numbers[9];

// Look-up table for whether a certain bitmap represents only one digit
extern uint16_t singles[512];

// for convenience: a look-up table mapping bitmapped digits back to real digits
extern uint16_t ps[512];

// number of set bits
extern uint16_t set_bits[512];

// number of trailing zeros
extern uint16_t tz[512];

// number of leading zeros
extern uint16_t lz[512];

// possible range given length and possible range (possibly with stranded digits)
extern uint16_t p_range[9][512];

// sure digits given length and possible range
extern uint16_t sure_digits[9][512];

#endif