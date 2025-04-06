#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 27 //without scrolling
#define LENGTH 115
#define WIDTH 31
#define DEFAULT_BOARD_X 60
#define DEFAULT_BOARD_Y 4
#define DEFAULT_LEGEND_X 20
#define DEFAULT_LEGEND_Y 4
#define BLACK_STONE 'X' 
#define WHITE_STONE 'O'
#define BLACK_STONE_COLOR LIGHTBLUE 
#define WHITE_STONE_COLOR LIGHTCYAN 
#define BORDER_COLOR WHITE
#define CURSOR_COLOR RED
#define BOARD_COLOR LIGHTGRAY
#define BOARD_BACKGROUND_COLOR BLACK


#include<stdio.h>
#include <cstring>
#include"conio2.h"


enum {
    EMPTY = '0',
    BORDER = '#',
    BLACK_S = 'B',
    WHITE_S = 'W',
};

struct cursor_t {
    struct {
        int x = DEFAULT_BOARD_X,
            y = DEFAULT_BOARD_Y;
    } screen;

    struct {
        int x = 0,
            y = 0;
    }v_screen; //visible screen

    struct {
        int x = 0,
            y = 0;
    } board;

    cursor_t* pointer = NULL;

} cursor;

struct gameState_t {
    struct {
        double white = 0.0,
            black = 0.0;
    } points;

    bool round = 0;

    gameState_t* pointer = NULL;

} gameState;

struct board_t {
    int size = 0;

    struct {
        char* ptr = NULL;
    } state;

    board_t* pointer = NULL;

} board;


void alloc_board(board_t* pointer) {
    board.state.ptr = new char[(board.size + 2) * (board.size + 2)];
}

void reset_board(board_t* pointer) { //clear board
    for (int i = 0; i < board.size + 2; i++) {
        for (int j = 0; j < board.size + 2; j++) {
            if (j == 0 or i == 0 or i == board.size + 1 or j == board.size + 1) {
                board.state.ptr[i * (board.size + 2) + j] = BORDER;
            }
            else {
                board.state.ptr[i * (board.size + 2) + j] = EMPTY;
            }
        }
    }
}

bool canOneBreathe(int x, int y, board_t* pointer) { //check if stone has any liberties left
    char c_stone = board.state.ptr[(x * (board.size + 2)) + y];
    if (board.state.ptr[(x) * (board.size + 2) + y + 1] == EMPTY or board.state.ptr[(x) * (board.size + 2) + y + 1] == c_stone or
        board.state.ptr[(x) * (board.size + 2) + y - 1] == EMPTY or board.state.ptr[(x) * (board.size + 2) + y - 1] == c_stone or
        board.state.ptr[(x + 1) * (board.size + 2) + y] == EMPTY or board.state.ptr[(x + 1) * (board.size + 2) + y] == c_stone or
        board.state.ptr[(x - 1) * (board.size + 2) + y] == EMPTY or board.state.ptr[(x - 1) * (board.size + 2) + y] == c_stone) {
        return true;
    }
    return false;
}

void place_stone(board_t* pointer, cursor_t* pointer2, gameState_t* pointer3) {
    char stone, stone_c;
    int color;
    if (gameState.round == 0) {
        stone = BLACK_S;
        stone_c = BLACK_STONE;
        color = BLACK_STONE_COLOR;
    }
    else {
        stone = WHITE_S;
        stone_c = WHITE_STONE;
        color = WHITE_STONE_COLOR;
    }

    if (board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y] == EMPTY)
    {
        if (board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y + 1] == EMPTY or board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y + 1] == stone or
            board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y - 1] == EMPTY or board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y - 1] == stone or
            board.state.ptr[(cursor.board.x + 1) * (board.size + 2) + cursor.board.y] == EMPTY or board.state.ptr[(cursor.board.x + 1) * (board.size + 2) + cursor.board.y] == stone or
            board.state.ptr[(cursor.board.x - 1) * (board.size + 2) + cursor.board.y] == EMPTY or board.state.ptr[(cursor.board.x - 1) * (board.size + 2) + cursor.board.y] == stone)
        {
            gotoxy(cursor.v_screen.x + 2, cursor.v_screen.y + 1);
            textcolor(color);
            putch(stone_c);

            do {
                char zn = getch();
                if (zn == 0x1b) {
                    break;
                }
                else if (zn == 0x0d) {
                    board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y] = stone;

                    int points = 0; //counting how many stones did we kill with current stone
                    if (canOneBreathe(cursor.board.x + 1, cursor.board.y, board.pointer) == false) {
                        board.state.ptr[(cursor.board.x + 1) * (board.size + 2) + cursor.board.y] = EMPTY;
                        points++;
                    }
                    if (canOneBreathe(cursor.board.x - 1, cursor.board.y, board.pointer) == false) {
                        board.state.ptr[(cursor.board.x - 1) * (board.size + 2) + cursor.board.y] = EMPTY;
                        points++;
                    }
                    if (canOneBreathe(cursor.board.x, cursor.board.y + 1, board.pointer) == false) {
                        board.state.ptr[(cursor.board.x) * (board.size + 2) + cursor.board.y + 1] = EMPTY;
                        points++;
                    }
                    if (canOneBreathe(cursor.board.x, cursor.board.y - 1, board.pointer) == false) {
                        board.state.ptr[(cursor.board.x) * (board.size + 2) + cursor.board.y - 1] = EMPTY;
                        points++;
                    }
                    gameState.round = !gameState.round;
                    if (stone == BLACK_S) {
                        gameState.points.black += points;
                    }
                    else {
                        gameState.points.white += points;
                    }
                    points = 0;
                    break;
                }

            } while (true);
        }
    }
}

void real_xy(cursor_t* pointer) { // cursor coordinates on board and on visible screen not whole console
    cursor.board = { (cursor.screen.x - DEFAULT_BOARD_X) / 2 + 1, cursor.screen.y - DEFAULT_BOARD_Y + 1 };

    if (cursor.screen.x > LENGTH) {
        cursor.v_screen.x = cursor.screen.x - (cursor.screen.x - LENGTH);
    }
    else {
        cursor.v_screen.x = cursor.screen.x;
    }
    if (cursor.screen.y > WIDTH) {
        cursor.v_screen.y = cursor.screen.y - (cursor.screen.y - WIDTH);
    }
    else {
        cursor.v_screen.y = cursor.screen.y;
    }
}

void reset_cursor(cursor_t* pointer) {
    cursor.screen.x = DEFAULT_BOARD_X;
    cursor.screen.y = DEFAULT_BOARD_Y;
}

void display_board(board_t* pointer, cursor_t* pointer2) {
    int x = DEFAULT_BOARD_X,
        y = DEFAULT_BOARD_Y;
    if (cursor.screen.x > LENGTH) {
        x = x - (cursor.screen.x - LENGTH);
        clrscr();
    }


    if (cursor.screen.y > WIDTH) {
        y = y - (cursor.screen.y - WIDTH);
        clrscr();
    }

    for (int i = 0; i < (board.size + 2); i++) { //display border
        for (int j = 0; j < board.size + 2; j++) {
            gotoxy(x + 2 * i, y + j);
            if (i == 0 or i == board.size + 1 or j == 0 or j == board.size + 1) {
                textbackground(BORDER_COLOR);
                putch(' ');
                if (j == 0 or j == board.size + 1) {
                    gotoxy(x + 2 * i + 1, y + j);
                    putch(' ');
                }
                if (i == 0) {
                    gotoxy(x - 1, y + j);
                    putch(' ');
                }
                else if (i == board.size + 1) {
                    gotoxy(x + (2 * i) + 1, y + j);
                    putch(' ');
                }
            }
        }
    }
    textbackground(BLACK); //delete old left-side border
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < board.size + 2; i++) {
            gotoxy(x - 2 - j, y + i);
            putch(' ');
        }
    }

    gotoxy(x, y); //display board
    textbackground(BOARD_BACKGROUND_COLOR);
    for (int i = 1; i < (board.size + 1); i++) {
        gotoxy(x + 1, y + i);
        putch(' ');
        for (int j = 1; j < board.size + 1; j++) {
            gotoxy(x + 2 * i, y + j);
            if (board.state.ptr[i * (board.size + 2) + j] == WHITE_S) {
                textcolor(WHITE_STONE_COLOR);
                putch(WHITE_STONE);
            }
            else if (board.state.ptr[i * (board.size + 2) + j] == BLACK_S) {
                textcolor(BLACK_STONE_COLOR);
                putch(BLACK_STONE);
            }
            else if (board.state.ptr[i * (board.size + 2) + j] == EMPTY) {
                textcolor(BOARD_COLOR);
                putch('+');
            }
            gotoxy(x + 2 * i + 1, y + j);
            putch(' ');
        }
    }
}


void display_cursor(cursor_t* pointer, board_t* pointer2) {
    int x = cursor.screen.x,
        y = cursor.screen.y;
    if (cursor.screen.x > LENGTH) {
        x = x - (cursor.screen.x - LENGTH);
    }
    if (cursor.screen.y > WIDTH) {
        y = y - (cursor.screen.y - WIDTH);
    }
    gotoxy(x + 2, y + 1);
    textcolor(CURSOR_COLOR);
    textbackground(BOARD_BACKGROUND_COLOR);
    if (board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y] == EMPTY) {
        putch('+');
    }
    else if (board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y] == WHITE_S) {
        putch(WHITE_STONE);
    }
    else if (board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y] == BLACK_S) {
        putch(BLACK_STONE);
    }
}

void display_legend(cursor_t cursor, gameState_t gameState) {
    int x = DEFAULT_LEGEND_X,
        y = DEFAULT_LEGEND_Y;
    char coords[32], black_score[32], white_score[32];
    int line = 2;
    textbackground(BLACK);
    textcolor(WHITE);
    gotoxy(x, y);
    //cputs("Mateusz Blach 193174");
    //gotoxy(x, y + line++);
    cputs("ARROWS = move cursor");
    gotoxy(x, y + line++);
    cputs("ENTER = confirm");
    gotoxy(x, y + line++);
    cputs("Esc = cancel");
    gotoxy(x, y + line++);
    cputs("i = place stone");
    gotoxy(x, y + line++);
    cputs("h = handicap");
    gotoxy(x, y + line++);
    cputs("n = new game");
    gotoxy(x, y + line++);
    cputs("s = save game");
    gotoxy(x, y + line++);
    cputs("l = load game");
    gotoxy(x, y + line++);
    cputs("q = quit");
    line++;
    gotoxy(x, y + line++);
    sprintf(coords, "Coordinates: ( %d,%d ) ", cursor.board.x, cursor.board.y);
    cputs(coords);
    putch(' '); //usuwamy zdublowany nawias
    gotoxy(x, y + line++);
    cputs("Move: ");
    if (gameState.round == 0)
        cputs("Black");
    else
        cputs("White");

    gotoxy(x, y + line++);
    textcolor(BLACK_STONE_COLOR);
    if (gameState.points.black == 1) sprintf(black_score, "Black (%c): %.1f point", BLACK_STONE, gameState.points.black);
    else sprintf(black_score, "Black (%c): %.1f points", BLACK_STONE, gameState.points.black);
    cputs(black_score);
    putch(' '); //usuwamy zbędną s-kę
    gotoxy(x, y + line++);
    textcolor(WHITE_STONE_COLOR);
    if (gameState.points.white == 1) sprintf(white_score, "White (%c): %.1f point", WHITE_STONE, gameState.points.white);
    else sprintf(white_score, "White (%c): %.1f points", WHITE_STONE, gameState.points.white);
    cputs(white_score);
    putch(' ');
    textcolor(WHITE);
    line++;
    /*gotoxy(x, y + line++);
    cputs("Sections:");
    gotoxy(x, y + line++);
    textcolor(LIGHTGREEN);
    cputs("(a) (b) (c) (d) (e) ");
    textcolor(LIGHTGREEN);
    cputs("(f) ");
    textcolor(RED);
    cputs("(g) ");
    gotoxy(x, y + line++);
    textcolor(LIGHTGREEN);
    cputs("(h) ");
    textcolor(RED);
    cputs("(i) ");
    textcolor(RED);
    cputs("(j) ");
    textcolor(LIGHTGREEN);
    cputs("(k) ");
    textcolor(RED);
    cputs("(l) ");
    textcolor(8);
    cputs("(m) ");
    textcolor(8);
    cputs("(n) ");*/
}

void get_size(board_t* pointer) { //get board size from player
    char zn = ' ', zn2 = ' ', custom[64], selected[64];
    bool valid = 0, end = 0; //did player select valid size 
    clrscr();
    textcolor(WHITE);
    do {
        gotoxy(1, 1);
        cputs("Select board size:\n1.9x9\n2.13x13\n3.19x19\n4.Custom");
        zn = getch();
        int size = 0;
        if (zn == '1') {
            size = 9;
            valid = 1;
        }
        else if (zn == '2') {
            size = 13;
            valid = 1;
        }
        else if (zn == '3') {
            size = 19;
            valid = 1;
        }
        else if (zn == '4') {
            do {
                clrscr();
                gotoxy(1, 1);

                sprintf(custom, "Enter custom size (max is %d):", MAX_SIZE);
                cputs(custom);
                zn = getche();
                zn2 = getche();
                if (zn2 == 0x0d) { //one digit scenario
                    size = (int)(zn - 48);
                }
                else {
                    size = ((int)(zn - 48)) * 10 + (int)(zn2 - 48);
                }

                if (size > 0 and size <= MAX_SIZE) {
                    valid = 1;
                    break;
                }
            } while (true);
        }
        if (valid == 1) {
            do {
                clrscr();
                gotoxy(1, 1);
                sprintf(selected, "Selected size: %d Enter - Confirm | ESC - Cancel", size);
                cputs(selected);
                zn = getch();
                if (zn == 0x1b) { //ESC
                    valid = 0;
                    clrscr();
                    break;
                }
                else if (zn == 0x0d) { //Enter
                    board.size = size;
                    end = 1;
                    break;
                }
            } while (true);
        }
    } while (!end);
    clrscr();
}

char* get_file_name(char* name) { //get file name from player
    textcolor(WHITE);
    clrscr();
    gotoxy(1, 1);
    cputs("Enter file name: ");
    char zn = ' ';
    int n = 0;
    do {
        zn = getche();
        if (zn == 0x0d) {
            name[n] = '.';
            name[n + 1] = 't';
            name[n + 2] = 'x';
            name[n + 3] = 't';
            name[n + 4] = '\0';
            break;
        }
        else if (zn == 0x1b) {
            break;
        }
        else if (zn == 0x08) {
            putch(' ');
            gotoxy(wherex() - 1, wherey());
            n--;
        }
        else {
            name[n] = zn;
            n++;
        }
    } while (true);
    return name;
}

void handle_file(bool b, board_t* pointer, gameState_t* pointer2, cursor_t* pointer3) { // b = 0 -> save game to file
    char name[64], size[4], b_points[32], w_points[32], zn;                                                         // b = 1 -> load game from file
    get_file_name(name);
    FILE* file;
    if (b == 0)
        file = fopen(name, "w");
    else
        file = fopen(name, "r");
    while (file == NULL) {
        clrscr();
        gotoxy(1, 1);
        cputs("Wrong name. ESC - go back | Any key - OK");
        zn = getch();
        if (zn == 0x1b) { //ESC
            break;
        }
        else {
            clrscr();
        }
        get_file_name(name);
    }
    if (file != NULL) {
        if (b == 0) {
            sprintf(size, "%d", board.size);
            fputs(size, file);
            fputc(' ', file);
            sprintf(b_points, "%.1f", gameState.points.black);
            fputc(' ', file);
            fputs(b_points, file);
            fputc(' ', file);
            sprintf(w_points, "%.1f", gameState.points.white);
            fputc(' ', file);
            fputs(w_points, file);
            fputc(' ', file);
            if (gameState.round)
                fputc('1', file);
            else
                fputc('0', file);
            fputc(' ', file);
            fprintf(file, "\n");
            for (int i = 0; i <= board.size + 1; i++) {
                for (int j = 0; j <= board.size + 1; j++) {
                    fputc(board.state.ptr[j * (board.size + 2) + i], file);
                }
                fprintf(file, "\n");
            }
            int fclose(FILE * file);
        }
        else {
            int size, round;
            double pb, pw;
            if (fscanf(file, "%d %lf %lf %d \n", &size, &pb, &pw, &round)) {
                board.size = size;
                gameState.points.black = pb;
                gameState.points.white = pw;
                gameState.round = round;
                board.state.ptr = new char[(board.size + 2) * (board.size + 2)];
                for (int i = 0; i <= board.size + 1; i++) {
                    for (int j = 0; j <= board.size + 1; j++) {
                        board.state.ptr[j * (board.size + 2) + i] = fgetc(file);
                    }
                    fgetc(file); //nowa linia
                }
            }
            int fclose(FILE * file);
        }
    }
    clrscr();
    cursor.screen = { DEFAULT_BOARD_X, DEFAULT_BOARD_Y };
}

void move_cursor(cursor_t* pointer) { //change cursor's position with arrows   
    char zn = getch();
    if (zn == 0x48 and cursor.screen.y - 1 >= DEFAULT_BOARD_Y) {
        cursor.screen.y--;
    }
    else if (zn == 0x50 and cursor.screen.y + 1 < DEFAULT_BOARD_Y + board.size) {
        cursor.screen.y++;
    }
    else if (zn == 0x4b and cursor.screen.x - 2 >= DEFAULT_BOARD_X) {
        cursor.screen.x -= 2;
    }
    else if (zn == 0x4d and cursor.screen.x + 2 < DEFAULT_BOARD_X + 2 * board.size) {
        cursor.screen.x += 2;
    }
    cursor.board.x = (cursor.screen.x - DEFAULT_BOARD_X) / 2 + 1;
    cursor.board.y = cursor.screen.y - DEFAULT_BOARD_Y + 1;
}

void new_game(board_t* pointer, cursor_t* pointer2, gameState_t* pointer3) { //start a new game
    char zn = ' ';
    clrscr();
    gotoxy(1, 1);
    textcolor(WHITE);
    cputs("Start new game. Enter - Confirm | ESC - Cancel");
    do {
        zn = getch();
        if (zn == 0x1b) {
            break;
        }
        else if (zn == 0x0d) {
            cursor.screen = { DEFAULT_BOARD_X, DEFAULT_BOARD_Y };
            get_size(board.pointer);
            reset_board(board.pointer);
            gameState.round = 0;
            gameState.points.white = 0.0;
            gameState.points.black = 0.0;
            break;
        }

    } while (true);
    clrscr();
    reset_cursor(cursor.pointer);
}

bool quit_game() {
    char zn = ' ';
    clrscr();
    gotoxy(1, 1);
    textcolor(WHITE);
    cputs("Are you sure you want to quit? Enter - Yes | ESC - No");
    do {
        zn = getch();
        if (zn == 0x0d) { //Enter
            textcolor(WHITE);
            textbackground(BLACK);
            clrscr();
            return 1;
        }
        else if (zn == 0x1b) { //ESC
            clrscr();
            return 0;
        }
    } while (zn != 0x1b and zn != 0x0d);
    clrscr();
    return 0;
}

bool can_handicap(board_t* pointer) { //handicap is possible only if the board is clear
    for (int i = 1; i <= board.size; i++) {
        for (int j = 1; j <= board.size; j++) {
            if (board.state.ptr[i * (board.size + 2) + j] != EMPTY)
                return 0;
        }
    }
    return 1;
}

void handicap(board_t* pointer, cursor_t* pointer2, gameState_t* pointer3) {
    if (can_handicap(board.pointer)) {
        char zn = ' ';
        double points = 0;
        do {
            real_xy(cursor.pointer);
            display_legend(cursor, gameState);
            display_board(board.pointer, cursor.pointer);
            display_cursor(cursor.pointer, board.pointer);
            zn = getch();
            if (zn == 0) {
                move_cursor(cursor.pointer);
            }
            else if (zn == 0x69 or zn == 0x49) {
                board.state.ptr[cursor.board.x * (board.size + 2) + cursor.board.y] = BLACK_S;
                points += 0.5;
            }
            else if (zn == 0x1b) {
                reset_board(board.pointer);
                break;
            }
            else if (zn == 0x0d) {
                gameState.round = !gameState.round;
                gameState.points.white += points;
                break;
            }
        } while (true);
    }
}

void clear() {
    textbackground(BLACK);
    for (int x = 1; x < LENGTH; x++) {
        for (int y = 1; y < WIDTH; y++) {
            gotoxy(x, y);
            putch(' ');
        }
    }
}
/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   with well defined identifiers */


int main() {
    // if the program is compiled in pure C, then we need to initialize
    // the library ourselves; __cplusplus is defined only if a C++ compiler
    // is used
#ifndef __cplusplus
    Conio2_Init();
#endif

    settitle("GO Game");
    _setcursortype(_NOCURSOR); //hide cursor

    bool play = 0;
    char zn = ' ';

    get_size(board.pointer);
    alloc_board(board.pointer);
    reset_board(board.pointer);

    do {

        real_xy(cursor.pointer);
        textbackground(BLACK);
        textcolor(WHITE);
        display_legend(cursor, gameState);
        display_board(board.pointer, cursor.pointer);
        display_cursor(cursor.pointer, board.pointer);

        zn = getch();

        if (zn == 0) {
            move_cursor(cursor.pointer);
        }
        else if (zn == 0x49 or zn == 0x69) { //place stone
            place_stone(board.pointer, cursor.pointer, gameState.pointer);
        }
        else if (zn == 0x4e or zn == 0x6e) { //new game
            new_game(board.pointer, cursor.pointer, gameState.pointer);
        }
        else if (zn == 0x48 or zn == 0x68) { //handicap
            handicap(board.pointer, cursor.pointer, gameState.pointer);
        }
        else if (zn == 0x53 or zn == 0x73) { //save game
            handle_file(0, board.pointer, gameState.pointer, cursor.pointer);
        }
        else if (zn == 0x4c or zn == 0x6c) { //load game
            handle_file(1, board.pointer, gameState.pointer, cursor.pointer);
        }
        else if (zn == 0x51 or zn == 0x71) { //quit
            play = quit_game();
        }
    } while (!play);
    delete[] board.state.ptr;
    _setcursortype(_NORMALCURSOR);  // show the cursor so it will be
    // visible after the program ends
    return 0;
}