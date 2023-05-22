#ifndef STR8TS_H
#define STR8TS_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "SMap.hpp"
#include "STable.hpp"


class Str8ts
{

private:

    // contains the initial position
    const STable table;
    SMap map;
    std::list<STable> solution_tables;

    // if this is set to true it will search for more than one solution
    bool solve_extensively = false;

public:

    // constructors
    Str8ts(std::string filename);

    // solving
    bool solve();
    bool solve(SMap* st_instance);

    // output methods
    void print_problem();
    void print_solutions();

    // return number of solutions
    int get_num_solutions() {
        return (int) solution_tables.size();  
    };

    // switch to extensive brute force search
    // finds all possible solutions
    void solve_ext(bool ext)
    {
        solve_extensively = ext;
    }

    // check if file could be loaded
    bool loaded() {
        return table.get_loaded();
    }
};

#endif
