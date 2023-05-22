#include "SMap.hpp"

#include "LUTs.hpp"
#include "runstats.hpp"

using namespace std;

extern bool opt_verbose;
/*
 * Constructors and such
 */
SMap::SMap(const STable table)
{
    fields = new uint16_t[9*9];
    blank = new bool[9*9];
    fixed = new bool[9*9];

    for (int i = 0; i < 9; i++) {
        sure_cand_col[i] = 0;
        sure_cand_row[i] = 0;
        for (int j = 0; j < 9; j++) {
            fields[i*9+j] = 511;
            blank[i*9+j] = 0;
            fixed[i*9+j] = 0;
            if (table.numbers[i][j]) {
                fields[i*9+j] = numbers[table.numbers[i][j]-1];
            }
            if (table.blanks[i][j]) {
                blank[i*9+j] = true;
                if (!table.numbers[i][j]) {
                    fields[i*9+j] = 0;
                }
            }
        }
    }

    // init straights
    char start, end;
    // rows
    for (char i = 0; i < 9; i++) {
        start = 0;
        do
        {
            while (start < 9 && blank[i*9+start])
                start++;
            if (start == 9)
                break;
            end = start + 1;
            while (end < 9 && !blank[i*9+end])
                end++;
            Straight* str8t = new Straight(
               this, fields+i*9+start, fields+i*9+end, 1, i*9 + start
           );
            row_straights[i].push_back(str8t);
            start = end + 1;
        }
        while (start < 9);
    }
    // columns
    for (char j = 0; j < 9; j++) {
        start = 0;
        do
        {
            while (start < 9 && blank[start*9+j])
                start++;
            if (start == 9)
                break;
            end = start + 1;
            while (end < 9 && !blank[end*9+j])
                end++;
            Straight* str8t = new Straight(
                this, fields+start*9+j, fields+end*9+j, 9, start*9 + j
            );
            col_straights[j].push_back(str8t);
            start = end + 1;
        }
        while (start < 9);
    }
}

SMap::SMap(const SMap& smp)
{
    // violation of the rules sets this to true
    hash_value = smp.hash_value;

    fields = new uint16_t[9*9];
    blank = new bool[9*9];
    fixed = new bool[9*9];

    memcpy(&sure_cand_row, &smp.sure_cand_row, 9*sizeof(uint16_t));
    memcpy(&sure_cand_col, &smp.sure_cand_col, 9*sizeof(uint16_t));
    memcpy(fields, smp.fields, 81*sizeof(uint16_t));
    memcpy(blank, smp.blank, 81*sizeof(bool));
    memcpy(fixed, smp.fixed, 81*sizeof(bool));

    for (int i = 0; i < 9; i++) {
        for (auto str8t : smp.row_straights[i]) {
            Straight* str = new Straight(*str8t);
            row_straights[i].push_back(str);
            // set pointers correctly
            str->map = this;
            str->start = str8t->start - smp.fields + fields;
            str->end = str8t->end - smp.fields + fields;
        }
        for (auto str8t : smp.col_straights[i]) {
            Straight* str = new Straight(*str8t);
            col_straights[i].push_back(str);
            // set pointers correctly
            str->map = this;
            str->start = str8t->start - smp.fields + fields;
            str->end = str8t->end - smp.fields + fields;
        }
    }
}

SMap& SMap::operator=(SMap mp)
{
    SMap *new_map = new SMap(mp);
    return *new_map;
}

SMap::~SMap()
{
    delete[] fields;
    delete[] blank;
    delete[] fixed;
    for (int i = 0; i < 9; i++) {
        for (auto str8t : row_straights[i])
            delete str8t;
        for (auto str8t : col_straights[i])
            delete str8t;
    }
}

///////////
// Rules //
///////////

bool SMap::apply_rules()
{
    StartStop  x = StartStop(__func__);
    change();

    bool no_change = false;
    while(!no_change) {
        if (remove_used_digits()) {
            goto violation_check;
        }
        if (straight_range()) {
            goto violation_check;
        }
        if (sure_candidates()) {
            goto violation_check;
        }
        if (sure_singles()) {
            goto violation_check;
        }
        if (stranded_digits()) {
            goto violation_check;
        }
        if (naked_pairs()) {
            goto violation_check;
        }
        if (make_sure()) {
            goto violation_check;
        }
        if (hidden_pairs()) {
            goto violation_check;
        }
        if (naked_groups()) {
            goto violation_check;
        }
        // arrives here going through all functions without change
        no_change = true;
        // jumps here if somethings has changed
        violation_check:
        if (!violation_free()) {
            if (opt_verbose) print();
            return false;
        }
    }
    return true;
}

bool SMap::change()
{
    int old = hash_value;
    hash_value = 0;
    for (int i = 0; i < 81; i++) 
        hash_value += set_bits[fields[i]];
    for (int i = 0; i < 9; i++) {
        hash_value -= set_bits[sure_cand_row[i]];
        hash_value -= set_bits[sure_cand_col[i]];
    }
    return !(old == hash_value);
}

bool SMap::remove_used_digits()
{
    StartStop  x = StartStop(__func__);
    for (int i = 0; i < 81; i++) {
        if(singles[fields[i]] && !fixed[i]) {
            char row = i%9;
            char col = i-row;
            uint16_t temp = ~fields[i];
            // row deletion
            for(int j = 0; j < 9; j++)
                fields[col+j] &= temp;
            // column deletion
            for(int j = 0; j < 81; j+=9)
                fields[row+j] &= temp;
            fields[i] = ~temp;
            fixed[i] = true;
        }
    }
    return change();
}

bool SMap::straight_range()
{
    StartStop  x = StartStop(__func__);
    for (int i = 0; i < 9; i++) {
        for (auto str : row_straights[i])
            str->check_range();
    }
    for (int i = 0; i < 9; i++) {
        for (auto str : col_straights[i])
            str->check_range();
    }
    return change();
}

bool SMap::sure_candidates()
{
    StartStop  x = StartStop(__func__);
    uint16_t sure = 0;
    // remove sure candidates of one straight from the others
    for (int i = 0; i < 9; i++) {
        sure = 0;
        // collect all sure candidates
        for (auto str : row_straights[i])
            sure |= str->sure;
        // remove them
        for (auto str : row_straights[i])
            str->sure_candidates(sure);
        sure_cand_row[i] = sure;
        sure = 0;
        // collect all sure candidates
        for (auto str : col_straights[i])
            sure |= str->sure;
        // remove them
        for (auto str : col_straights[i])
            str->sure_candidates(sure);
        sure_cand_col[i] = sure;
    }
    return change();
}

bool SMap::sure_singles()
{
    StartStop  x = StartStop(__func__);
    for (int i = 0; i < 9; i++) {
        for (auto str : row_straights[i])
            str->sure_singles();
        for (auto str : col_straights[i])
            str->sure_singles();
    }
    return change();
}

bool SMap::stranded_digits()
{
    StartStop  x = StartStop(__func__);
    for (int i = 0; i < 9; i++) {
        for (auto str : row_straights[i])
            str->stranded_digits();
        for (auto str : col_straights[i])
            str->stranded_digits();
    }
    return change();
}

bool SMap::naked_pairs()
{
    StartStop  x = StartStop(__func__);
    for (int i = 0; i < 9; i++) {
        for (auto str : row_straights[i])
            str->naked_pairs();
        for (auto str : col_straights[i])
            str->naked_pairs();
    }
    return change();
}

bool SMap::naked_groups()
{
    // for pairs
    StartStop  x = StartStop(__func__);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (set_bits[fields[9*i+j]] == 2) {
                for (int k = j+1; k < 9; k++) {
                    if (fields[9*i+j] == fields[9*i+k]) {
                        // subtract those from all other fields
                        for (int l = 0; l < 9; l++)
                        {
                            if (l != j && l != k)
                                fields[9*i+l] &= ~fields[9*i+j];
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (set_bits[fields[9*j+i]] == 2) {
                for (int k = j+1; k < 9; k++) {
                    if (fields[9*j+i] == fields[9*k+i]) {
                        // subtract those from all other fields
                        for (int l = 0; l < 9; l++)
                        {
                            if (l != j && l != k)
                                fields[9*l+i] &= ~fields[9*j+i];
                        }
                    }
                }
            }
        }
    }
    return change();
}

bool SMap::make_sure()
{
    StartStop  x = StartStop(__func__);
    uint16_t sure = 0;
    for (int i = 0; i < 9; i++) {
        sure = 0;
        // collect all sure candidates
        for (auto str : row_straights[i])
            sure |= str->sure;
        sure_cand_row[i] = sure;
        sure = 0;
        // collect all sure candidates
        for (auto str : col_straights[i])
            sure |= str->sure;
        sure_cand_col[i] = sure;
    }
    return change();
}

bool SMap::hidden_pairs()
{
    StartStop  x = StartStop(__func__);
    for (int i = 0; i < 9; i++) {
        for (auto str : row_straights[i])
            str->hidden_pairs();
        for (auto str : col_straights[i])
            str->hidden_pairs();
    }
    return change();
}


/////////////////////////
// Auxiliary functions //
/////////////////////////

// returns how many numbers are fixed already
char SMap::number_fixed()
{
    char count = 0;
    for (int i = 0; i < 81; i++)
        if (singles[fields[i]] || blank[i])
            count++;
    return count;
}

bool SMap::solved()
{
    int count_fixed = number_fixed();
    if (count_fixed == 81)
        return true;
    else
        return false;
}

bool SMap::violation_free()
{
    extern bool opt_verbose;
    
    for (int i = 0; i < 81; i++)
        if (!fields[i] && !blank[i]) {
            if (opt_verbose) {
                
                cout << "ABCDEFGHJ"[i/9] << "123456789"[i%9] << " is dead:" << endl;
            }
            return false;
        }
    for (int i = 0; i < 9; i++) {
        for (auto str : row_straights[i])
            if (str->range_violation())
                return false;
        for (auto str : col_straights[i])
            if (str->range_violation())
                return false;
    }
    return true;
}

// returns a list for backtracking
std::list<SMap*> SMap::choose()
{
    int i;
    // backtracking a field with a small number of possible digits seems to be advantagous.
    int choices = 10;
    int field = -1;
    for (i = 0; i < 81; i++)
        if (!fixed[i] && !blank[i]) {
            if (set_bits[fields[i]] < choices) {
                choices = set_bits[fields[i]];
                field = i;
            }
        }
    i = field;

    list<SMap*> list_st;
    for (int j = 0; j < 9; j++) {
        if (fields[i] & numbers[j]) {
            SMap* st = new SMap(*this);
            st->fields[i] = numbers[j];
            list_st.push_back(st);
        }
    }
    return list_st;
}

////////////
// Output //
////////////

void SMap::print()
{
    STable new_st = get_table();
    new_st.print();
}

STable SMap::get_table()
{
    char numbers[9][9] = {};
    bool blanks[9][9] = {};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (blank[i*9+j])
                blanks[i][j] = true;
            if (singles[fields[i*9+j]])
                numbers[i][j] = (int)ps[fields[i*9+j]];
        }
    }
    return STable(numbers, blanks);
}
