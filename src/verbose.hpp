//
//  verbose.hpp
//
// Um den Original-Code für zusätzliche Nachrichten nicht zu sehr zu verunstalten
// sind hier ein paar Service-Routinen veröffentlicht.
// Implementiert ist auch  Straight::range_violation_verbose().

#ifndef verbose_h
#define verbose_h
#include <string>

char row_as_char(const int i);
char col_as_char(const int i);
std::string coordinates(const int i);

#endif /* verbose_h */
