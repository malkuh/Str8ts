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
    printf("\u2554");
    for (int j = 0; j < 9; j++) {
        printf("\u2550");
    }
    printf("\u2557\n");
    for (int i = 0; i < 9; i++) {
        // siding
        printf("\u2551");
        for (int j = 0; j < 9; j++) {
            if (blanks[i][j]) {
                if (numbers[i][j]) {
                    cout << (char)(numbers[i][j]+'a'-1);
                }
                else {
                    cout << "#";
                }
            }
            else {
                if (numbers[i][j]) {
                    cout << (int)numbers[i][j];
                }
                else {
                    cout << ".";
                }
            }
        }
        // siding
        printf("\u2551");
        cout << endl;
    }

    // closing table
    printf("\u255A");
    for (int j = 0; j < 9; j++) {
        printf("\u2550");
    }
    printf("\u255D\n");
}

bool STable::get_loaded() const
{
    return loaded;
}

// deprecated input formats

// STable::STable(string filename)
// {
//     bool reverse = true;
//     ifstream file(filename, ios::in);
//     if(file.is_open()) {
//         char c;
//         if (reverse) {
//             for (int i = 0; i < 9; i++) {
//                 for (int j = 0; j < 9; j++) {
//                     do
//                         file >> c;
//                     while('0' > c || c > '9');
//                     if (c == '0')
//                         numbers[i][j] = 0;
//                     else
//                         numbers[i][j] = char (c - '0');
//                 }
//             }
//             for (int i = 0; i < 9; i++) {
//                 for (int j = 0; j < 9; j++) {
//                     do
//                         file >> c;
//                     while('0' > c || c > '9');
//                     if('1' == c)
//                         blanks[i][j] = true;
//                     else
//                         blanks[i][j] = false;
//                 }
//             }
//         }
//         else
//         {
//             for (int i = 0; i < 9; i++) {
//                 for (int j = 0; j < 9; j++) {
//                     do
//                         file >> c;
//                     while('0' > c || c > '9');
//                     if('1' == c)
//                         blanks[i][j] = true;
//                     else
//                         blanks[i][j] = false;
//                 }
//             }
//             for (int i = 0; i < 9; i++) {
//                 for (int j = 0; j < 9; j++) {
//                     do
//                         file >> c;
//                     while('0' > c || c > '9');
//                     if (c == '0')
//                         numbers[i][j] = 0;
//                     else
//                         numbers[i][j] = char (c - '0');
//                 }
//             }
//         }
//         file.close();
//     }
//     else
//         cout << "Error reading file!" << endl;
// }
//
// STable::STable(string filename)
// {
//     ifstream file(filename, ios::in);
//     if(file.is_open()) {
//         char c;
//         for (int i = 0; i < 9; i++) {
//             for (int j = 0; j < 9; j++) {
//                 do
//                     file >> c;
//                 while('0' > c || c > '9');
//                 if('1' == c)
//                     blanks[i][j] = true;
//                 else
//                     blanks[i][j] = false;
//             }
//         }
//         for (int i = 0; i < 9; i++) {
//             for (int j = 0; j < 9; j++) {
//                 do
//                     file >> c;
//                 while('0' > c || c > '9');
//                 if (c == '0')
//                     numbers[i][j] = 0;
//                 else
//                     numbers[i][j] = char (c - '0');
//             }
//         }
//         file.close();
//     }
//     else
//         cout << "Error reading file!" << endl;
// }

