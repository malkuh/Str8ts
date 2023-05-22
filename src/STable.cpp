#include "STable.hpp"

using namespace std;


STable::STable(string filename)
{
    ifstream file(filename, ios::in);
    if(file.is_open()) {
        // TODO: make this a little more robust against errors in input file.
        char c;
        loaded = true;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if(file.eof()) {
                     loaded = false;
                }
                else {
                    do {
                        file >> c;
                        if (c == '.') {
                            numbers[i][j] = 0;
                            blanks[i][j] = false;
                            break;
                        }
                        if (c == '#') {
                            numbers[i][j] = 0;
                            blanks[i][j] = true;
                            break;
                        }
                        if ('0' < c && c <= '9') {
                            numbers[i][j] =  char (c - '0');
                            blanks[i][j] = false;
                            break;
                        }
                        if ('i' >= c && c >= 'a') {
                            numbers[i][j] =  char (c - 'a' + 1);
                            blanks[i][j] = true;
                            break;
                        }
                    }
                    while(!file.eof());
                }
            }
        }
        file.close();
    }
    else {
        cout << "Error reading file!" << endl;
    }
    
}


STable::STable(char nums[9][9], bool blks[9][9])
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            numbers[i][j] = nums[i][j];
            blanks[i][j] = blks[i][j];
        }
    }
}

void STable::print() const
{
    // opening the table
    printf("\u2554\u2550");
    for (int j = 0; j < 9; j++) {
        printf("\u2550\u2550");
    }
    printf("\u2557\n");
    for (int i = 0; i < 9; i++) {
        // siding
        printf("\u2551 ");
        for (int j = 0; j < 9; j++) {
            if (blanks[i][j]) {
                if (numbers[i][j]) {
                    cout << "\u001b[7m" << (int) numbers[i][j] << "\u001b[;m ";
                    // cout << (char)(numbers[i][j]+'a'-1) << " ";
                }
                else {
                    cout << "█ ";
                }
            }
            else {
                if (numbers[i][j]) {
                    cout << (int)numbers[i][j] << " ";
                }
                else {
                    cout << ". ";
                }
            }
        }
        // siding
        printf("\u2551");
        cout << endl;
    }

    // closing table
    printf("\u255A\u2550");
    for (int j = 0; j < 9; j++) {
        printf("\u2550\u2550");
    }
    printf("\u255D\n");
}

bool STable::get_loaded() const
{
    return loaded;
}

