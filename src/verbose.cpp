//
//  verbose.cpp
//
//

#include "verbose.hpp"

bool Verbose::on; // You need to actually define the static member somewhere
// see: https://stackoverflow.com/questions/272900/undefined-reference-to-static-class-member

std::string Verbose::coordinates(const int i) {
    return { Verbose::ROWS[i/9], Verbose::COLS[i%9] };
}

char Verbose::row_as_char(const int i) {
    return Verbose::ROWS[i];
}
