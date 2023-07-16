//
//  verbose.hpp
//
// Some helper declarations for verbose messages.

#ifndef verbose_h
#define verbose_h
#include <string>

struct Verbose {
    
    static bool on;
    constexpr static char ROWS[] = "ABCDEFGHJ";
    constexpr static char COLS[] = "123456789";
    static char row_as_char(const int row_num);
    static char col_as_char(const int col_num);
    static std::string coordinates(const int i);
private:
    Verbose() {};
    
};

#endif /* verbose_h */
