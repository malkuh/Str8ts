//
//  verbose.cpp
//
//

#include <stdio.h>

#include "verbose.hpp"
#include "Straight.hpp"
#include "SMap.hpp"

extern bool opt_verbose;

static const char ROWS[] = "ABCDEFGHJ";
static const char COLS[] = "123456789";

void Straight::range_violation_verbose() {
    
    if (!opt_verbose) return;
    
    std::string straight = "";
    if ( incr == 1 ) {
        // row
        const int col = starts_at % 9;
        straight += ROWS[starts_at/9];
        for ( int i = col; i < col + length; i++) {
            straight += COLS[i];
        }
        
    } else {
        // col
        const int row = starts_at / 9;
        for ( int i = row; i < row + length; i++) {
            straight += ROWS[i];
        }
        straight += COLS[starts_at%9];
    }
    
    std::cout << "range violation in " << straight << ":" << std::endl;
}

std::string coordinates(const int i) {
    return { ROWS[i/9], COLS[i%9] };
}

char row_as_char(const int i) {
    return ROWS[i];
}
