//
//  prompt.cpp
//

#include "prompt.hpp"
#include "verbose.hpp"

#include <iostream>
#include <unordered_set>

using namespace std;
namespace {
    const string VALID = "123456789" "abcdefghi" "#.";
    const unordered_set<char> VALIDS( begin(VALID), end(VALID));

    bool process_line (const string & line, string & lines) {
        if ( line.size() < 9 ) return false;
        
        string result {};
        
        for( const auto & c: line ) {
            if (VALIDS.find(c) == VALIDS.end()) continue;
            result += c;
            
        }
        if ( result.size() != 9 ) {
            return false;
            
        }
        lines += result;
        return true;
    }
};

void prompt_puzzle(string & puzzle) {
    string the_line;
    
    auto row = 0;
    while ( row < 9 ) {
        cout << row_as_char(row) <<  " > ";
        getline(cin, the_line);
        if ( cin.eof()) {
            cerr << "eof\n";
            break;
        }
        if (! process_line( the_line, puzzle )) {
            continue;
        }
        cout << "    ";
        for ( auto c = puzzle.begin() + 9*row, c_end = puzzle.end(); c != c_end; ++c) {
            if ('a' <= *c and *c <= 'i' ) {
                cout << "\u001b[7m" << (char) (*c + '1' - 'a') << "\u001b[;m";
            } else if ( *c == '#' ) {
                cout << "\u001b[7m" << ' ' << "\u001b[;m";
            } else {
                cout << *c;
            }
        }
        cout << " ok?";
        string ok;
        getline(cin, ok);
        if (ok != "" and ok != "y" ) {
            continue;
        }
        row += 1;
    }
}
