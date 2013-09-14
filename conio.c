#include "conio.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32

#include <windows.h>

void clrscr() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int x = GetLargestConsoleWindowSize(h).X;
    int y = GetLargestConsoleWindowSize(h).Y;
    COORD c;
    c.X = 0;
    c.Y = 0;
    FillConsoleOutputCharacter(h, ' ', x * y, c, NULL);
    SetConsoleCursorPosition(h, c);
}

void gotoxy(int x, int y) {
    COORD c;
    c.X = x - 1;	// ANSI, and Borland coordinates are 1-based, while WINAPI coordinates are 0-based, that's why we need some conversion.
    c.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Query cursor position
COORD getCursorPos() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    return csbiInfo.dwCursorPosition;
}

int wherex() {
    return getCursorPos().X + 1;
}

int wherey() {
    return getCursorPos().Y + 1;
}

void cursorup() {
    gotoxy(wherex(), wherey() - 1);
}

void cursordown() {
    gotoxy(wherex(), wherey() + 1);
}

void cursorfwd() {
    gotoxy(wherex() + 1, wherey());
}

void cursorback() {
    gotoxy(wherex() - 1, wherey());
}

void clreol() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int x = GetLargestConsoleWindowSize(h).X;
    int y = GetLargestConsoleWindowSize(h).Y;

    COORD c = getCursorPos();
	
    FillConsoleOutputCharacter(h, ' ', x - c.X, c, NULL);
    SetConsoleCursorPosition(h, c);
	
}

void getpassword(const char maskchar, char* s, int len_s) {
    char key;
    int i;
    for (i = 0; i < len_s; i++) {
        key = getch();
        if (key == '\010') {
            if (i > 0) {
                gotoxy(wherex() - 1, wherey());
                putchar(' ');
                gotoxy(wherex() - 1, wherey());
                i -= 2;
            }
            continue;
        }
        if (key == '\015')
            break;
        s[i] = key;
        putchar(maskchar);
    }
    s[i] = 0;
    printf("\n");
}

int fg_color, bg_color;

void textcolor(int newcolor) {
    fg_color = newcolor;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color);
}

void textbackground(int newcolor) {
    bg_color = newcolor << 4;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color);
}

void textdefault() {
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
}

void cursoroff() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ccInfo;
    GetConsoleCursorInfo(h, &ccInfo);
    ccInfo.bVisible = 0;
    SetConsoleCursorInfo(h, &ccInfo);
}

void cursoron() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ccInfo;
    GetConsoleCursorInfo(h, &ccInfo);
    ccInfo.bVisible = 1;
    SetConsoleCursorInfo(h, &ccInfo);
}

#else

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios _old, _new;

/* Initialize new terminal i/o settings */

void initTermios(int echo) {
    tcgetattr(0, &_old);                 /* grab old terminal i/o settings */
    _new = _old;                         /* make new settings same as old settings */
    _new.c_lflag &= ~ICANON;             /* disable buffered i/o */
    _new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &_new);        /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */

void resetTermios(void) {
    tcsetattr(0, TCSANOW, &_old);
}

/* Read 1 character - echo defines echo mode */

char getch_(int echo) {
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

int bgcolor;

/*
 * DSR - Device Status Report
 * Queries cursor position, and returns to STDIN in the following format: ESC[<vertical position>;<horizontal position>R
 * This function calls DSR, and parses its output into an int.
 */
int dsr(int xy) {
    printf("\033[6n");
    getch();	// ESC
    getch();	// [
    
    int i = 0;
    char c = 0;
    char x[3];
    char y[3];
    while (c != ';') {
        c = getch();
        y[i++] = c;
    }
    y[--i] = 0;
    i = 0;
    while (c != 'R') {
        c = getch();
        x[i++] = c;
    }
    x[--i] = 0;
    
    if (xy) return atoi(x);
    else    return atoi(y);
}

void clreol() {
    printf("\033[1K");
}


void clrscr() {
    printf("\033[2J\033[1;1f");
}


char getch() {
    return getch_(0);
}


char getche() {
    return getch_(1);
}



int kbhit(void) {
    int ch;
    initTermios(0);
    ch = getchar(); 
    resetTermios();

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}


 
void getpassword(const char maskchar, char* s, int len_s) {
    char key;
    int i;
    for (i = 0; i < len_s; i++) {
        key = getch();
        if (key == '\177') {
            if (i > 0) {
                printf("\033[1D \033[1D");
                i -= 2;
            }
            continue;
        }
        if (key == '\012')
            break;
        s[i] = key;
        putchar(maskchar);
    }
    s[i] = 0;
    printf("\n");
}


void gotoxy(int x, int y) {
    printf("\033[%d;%df", y, x);
}


void cursorup() {
    printf("\033[1A");
}


void cursordown() {
    printf("\033[1B");
}


void cursorfwd() {
    printf("\033[1C");
}


void cursorback() {
    printf("\033[1D");
}


void textcolor(int newcolor) {
    int c = newcolor % 16;
    if (c < 8) {
        printf("\033[3%d;0m\033[3%dm", c, c);
    } else {
        int d = c % 8;
        printf("\033[3%d;1m\033[3%dm", d, d);
    }
    textbackground(bgcolor);
}


void textbackground(int newcolor) {
    int c = newcolor % 8;
    bgcolor = c;
    printf("\033[4%dm", c);
}


void textdefault() {
    printf("\033[39;0m\033[49m");
}


void cursoroff() {
    printf("\033[?25l");
}


void cursoron() {
    printf("\033[?25h");
}


int wherex() {
    return dsr(1);
}

int wherey() {
    return dsr(0);
}

#endif

void delay(int t) {
    time_t starttime;
    time_t curtime;
    time(&starttime);
    
    do {
        time(&curtime);
    } while (curtime - starttime < t);
}
