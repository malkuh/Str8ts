//
//  stats.hpp
//  Str8tsSolver
//
//  Created by Hans-Peter Bodden on 14.05.23.
//

#ifndef stats_hpp
#define stats_hpp

#include <string>
#include <stdio.h>

struct StatFields {
    int successes = 0;
    double elapsed = 0;
};

struct Stat {
    int count = 0;
    StatFields fields;
    void operator+=(const StatFields & increment);
};

class Stats {
    std::unordered_map<std::string, Stat> the_map;
public:
    void update(const std::string & the_func, const StatFields & the_stat);
    void print_map();
};

#endif /* stats_hpp */
