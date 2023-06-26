#ifndef SMAP_H
#define SMAP_H

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <inttypes.h>
#include <cstring>
#include <array>
#include <memory>

#include "LUTs.hpp"
#include "Straight.hpp"
#include "STable.hpp"


struct SMap
{
    // Attributes //
    // Bitmaps for each field.
    // If n is a possible digit in field i, then 2^(n-1) & fields[i] is true.
    std::array<Candidates, 9*9> fields;
    std::array<bool, 9*9> blank;
    std::array<bool, 9*9> fixed;
    int hash_value = 0;

    std::array<Candidates, 9> sure_cand_row;
    std::array<Candidates, 9> sure_cand_col;

    // saves  each straight
    std::list<std::unique_ptr<Straight>> row_straights[9];
    std::list<std::unique_ptr<Straight>> col_straights[9];

    SMap(const STable table);
    SMap(std::string filename);
    SMap(const SMap& st_arg);

    SMap& operator=(SMap mp);
    ~SMap();

    // apply rules
    bool apply_rules();

    // heuristic rules; return whether they were successful.
    bool remove_used_digits();
    bool straight_range();
    bool sure_candidates();
    bool sure_singles();
    bool stranded_digits();
    bool naked_pairs();
    bool naked_groups(); // whole row and column wise
    bool make_sure();
    bool hidden_pairs();

    bool change();

    // returns how many numbers are fixed already
    char number_fixed();

    // returns whether all non-blank fields have fixed numbers and if the solution complies the rules
    bool solved();
    bool violation_free();

    // returns a list for backtracking
    std::list<std::unique_ptr<SMap>> choose();

    // Output
    void print();
    STable get_table();
};

#endif
