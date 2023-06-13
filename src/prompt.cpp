//
//  prompt.cpp
//

#include "prompt.hpp"

#include <ncurses.h>

struct Cell {
    WINDOW * win = nullptr;
    char content = ' ';
    bool blocked = false;
    static const auto h = 3;
    static const auto w = 5;
};

static void mark_cell(WINDOW * win, const bool selected);
static char output_cell(const Cell &);
static void fill_cell(WINDOW * win);

void prompt_puzzle(std::string & puzzle) {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0); // kein Cursour
    noecho();
    
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    const auto color_normal = COLOR_PAIR(1);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    const auto color_block = COLOR_PAIR(3);
    init_pair(4, COLOR_WHITE, COLOR_YELLOW);
    const auto color_outer = COLOR_PAIR(4);
    
    mvprintw(0, 16, "Press q to exit");
    refresh();
    
    auto outer_top = newwin(1, 1 + 9 * Cell::w, 4, 4);
    wbkgd(outer_top, color_outer);
    for (auto i = 0; i!= 9 ; ++i) {
        mvwaddch(outer_top, 0, 3 + i * Cell::w, '1' + i);
    }
    wrefresh(outer_top);
    auto outer_left = newwin(9 * Cell::h, 3, 5, 2);
    wbkgd(outer_left, color_outer);
    for ( auto i = 0; i != 9; ++i ) {
        mvwaddch(outer_left, 1 + i * Cell::h, 1, 'A' + i);
    }
    wrefresh(outer_left);
    auto outer_right = newwin(9 * Cell::h, 3, 5, 5 + 9 * Cell::w);
    wbkgd(outer_right, color_outer);
    wrefresh(outer_right);
    auto outer_bottom = newwin(1,1 + 9 * Cell::w + 1, 5 + 9 * Cell::h, 4);
    wbkgd(outer_bottom, color_outer);
    wrefresh(outer_bottom);
    
    Cell cell[9][9];
    for ( auto row = 0; row != 9; ++row ) {
        for ( auto col = 0; col != 9; ++col ) {
            auto startx = 5 + Cell::w * col;
            auto starty = 5 + Cell::h * row;
            auto win = newwin(Cell::h, Cell::w, starty, startx);
            fill_cell(win);
            
            cell[row] [col].win = win;
            if (row == 0 and col == 0) {
                mark_cell(win, true);
            }
            wbkgd(win, color_normal);
            wrefresh(win);
        }
    }
        
    auto selected_row = 0, selected_col = 0;
    
    int key;
    while( (key = getch()) != 'q' ) {
        
        auto selected_row_old = selected_row;
        auto selected_col_old = selected_col;
        switch(key)
        {    case KEY_UP:
                selected_row = (selected_row + 9 - 1 ) % 9;
                break;
            case KEY_DOWN:
                selected_row = (selected_row + 9 + 1 ) % 9;
                break;
            case KEY_LEFT:
                selected_col = (selected_col + 9 - 1 ) % 9;
                break;
            case KEY_RIGHT:
                selected_col = (selected_col + 9 + 1 ) % 9;
                break;
        }
        auto current_cell = & cell[selected_row][selected_col];
        const auto current_win = current_cell->win;
        
        if (selected_col == selected_col_old and selected_row == selected_row_old) {
            // keine Bewegung:
            if ( '1' <= key and key <= '9' and current_cell->content != key) {
                current_cell->content = key;
                mvwaddch(current_win, 1, 2, key);
            } else if (key == '#') {
                current_cell->blocked = !current_cell->blocked;
                const auto color = current_cell->blocked ? color_block : color_normal;
                wbkgd(current_win, color);
                mvwaddch(current_win, 1, 2, current_cell->content); // ???
            } else if (key <= '0') {
                current_cell->content = ' ';
                mvwaddch(current_win, 1, 2, ' ');
            }
        } else {
            // Bewegung
            auto previous_cell = & cell[selected_row_old][selected_col_old];
            const auto previous_win = previous_cell->win;
            
            mark_cell(previous_win, false);
            mark_cell(current_win, true);
            
            if (current_cell->blocked) {
                wbkgd(current_win, color_block);
            } else {
                wbkgd(current_win, color_normal);
            }
            if (previous_cell->blocked) {
                
                wbkgd(previous_win, color_block);
            } else {
                wbkgd(previous_win, color_normal);
            }
            wrefresh(previous_win);
        }
        mvprintw(2, 14,
                 "Selected %c%c %s < %c >",
                 "ABCDEFGHI"[selected_row],
                 "123456789"[selected_col],
                 current_cell->blocked ? "block":
                 current_cell->content > '0' ? "!set!" :
                 "empty",
                 current_cell->content
                 );
        wrefresh(current_win);
            
    }
        
    endwin();            /* End curses mode          */
    
    for (auto row = 0; row != 9; ++row) {
        for (auto col = 0; col !=9; ++col) {
            auto c = & cell[row][col];
            puzzle += output_cell(*c);
        }
    }
}
static void fill_cell(WINDOW * win) {
    mvwaddch(win, 0, 0, ACS_ULCORNER);
    for (auto col = 1; col < (Cell::w - 1); ++col) {
        mvwaddch(win, 0, col, ACS_HLINE);
        mvwaddch(win, Cell::h - 1, col, ACS_HLINE);
    }
    mvwaddch(win, 0, Cell::w - 1, ACS_URCORNER);
    
    for (auto row = 1; row < (Cell::h - 1); ++row) {
        mvwaddch(win, row, 0, ACS_VLINE);
        mvwaddch(win, row, Cell::w - 1, ACS_VLINE);
    }
    mvwaddch(win, Cell::h - 1, 0, ACS_LLCORNER);
    mvwaddch(win, Cell::h - 1, Cell::w - 1, ACS_LRCORNER);
}
static void mark_cell(WINDOW * win, const bool selected) {
    if (selected) {
        mvwaddch(win, Cell::h / 2, 0, '>');
        mvwaddch(win, Cell::h / 2, Cell::w - 1, '<');
    } else {
        mvwaddch(win, Cell::h / 2, 0, ACS_VLINE);
        mvwaddch(win, Cell::h / 2, Cell::w - 1, ACS_VLINE);
    }
}

static char output_cell(const Cell & cell) {
    if (cell.blocked) {
        return cell.content > '0' ? cell.content + 'a' - '1': '#';
    }
    return cell.content > '0' ? cell.content : '.';
}
