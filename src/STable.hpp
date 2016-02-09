#ifndef STABLE_H
#define STABLE_H

#include<iostream>
#include<fstream>
#include<string>

struct STable
{
    char numbers[9][9] = {};
    bool blanks[9][9] = {};
    bool loaded = false;

    STable(std::string filename);
    STable(char nums[9][9], bool blks[9][9]);

    bool get_loaded() const;

    void print() const;
    void print(int c) const;
};

#endif