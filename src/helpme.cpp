//
//  helpme.cpp
//  str8ts
//

#include "helpme.hpp"

#include <iostream>
#include <string>

namespace {

    const auto helptext = {
        "",
        "Usage:",
        " str8ts [-v] [-t] [-i] [problem file ...]",
        "",
        "Options:",
        " -v : Be verbose",
        " -t : Generate call counts",
        " -i : Enter problem row wise at command line",
        " -a : Find all solutions (max 16).",
        
        "",
        "Problem file format:",
        "",
        "#f9.#..##",
        ".........",
        "..ai..#.6",
        "1...#..##",
        "#.......#",
        "h#..#...2",
        "..#..##..",
        "..3...8..",
        "##..#..g#",

        "",
        "that is '.' for an empty field, '#' for a black field, and",
        "'123456789' and 'abcdefghi' for digits",
        "on white or black fields, respectively.",

    };
};

void givehelp(void) {
    
    std::cerr << " Str8ts Solver\n\n\tcompiled at " << __DATE__ << ", " << __TIME__ << " cpp=" << __cplusplus << '\n';
    
    for (const auto & line: helptext) {
        
        const char indent = isupper(line[0]) ? ' ' : '\t';
        
        std::cerr << indent << line << '\n';
        
    }
    
}

