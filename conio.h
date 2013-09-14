/******************************************************************************
 * C/C++ module for handling console input/output.                            *
 * This is a cross-platform implementation of a mostly Borland compatible     *
 * conio.h (some features are missing and some features added).               *
 *                                                                            *
 * (c) 2013 by kemi                                                           *
 * This is free software, and isn't covered by any license. You may freely    *
 * use, distribute, or modify it as you like.                                 *
 ******************************************************************************/

#ifndef _CONIO_H_
#define _CONIO_H_

#ifdef _WIN32
#include <conio.h>
#endif

/**colors**/

/*
 * Please note that color constants are different for ANSI compatible systems, and Windows.
 * For cross-platform compatibility please use e. g. textcolor(GREEN) instead of textcolor(2).
 */

#ifndef _WIN32	// ANSI colors

static const int BLACK        =  0;
static const int RED          =  1;
static const int GREEN        =  2;
static const int BROWN        =  3;
static const int BLUE         =  4;
static const int MAGENTA      =  5;
static const int CYAN         =  6;
static const int LIGHTGRAY    =  7;
static const int DARKGRAY     =  8;
static const int LIGHTRED     =  9;
static const int LIGHTGREEN   = 10;
static const int YELLOW       = 11;
static const int LIGHTBLUE    = 12;
static const int LIGHTMAGENTA = 13;
static const int LIGHTCYAN    = 14;
static const int WHITE        = 15;

#else	// Windows colors

static const int BLACK        =  0;
static const int BLUE         =  1;
static const int GREEN        =  2;
static const int CYAN         =  3;
static const int RED          =  4;
static const int MAGENTA      =  5;
static const int BROWN        =  6;
static const int LIGHTGRAY    =  7;
static const int DARKGRAY     =  8;
static const int LIGHTBLUE    =  9;
static const int LIGHTGREEN   = 10;
static const int LIGHTCYAN    = 11;
static const int LIGHTRED     = 12;
static const int LIGHTMAGENTA = 13;
static const int YELLOW       = 14;
static const int WHITE        = 15;

#endif
/**end colors**/


/**
 * Clears all characters from the cursor position to the end of the line without moving the cursor.
 */
void clreol();

/**
 * Clears the entire screen, and moves the cursor to the top left corner.
 */
void clrscr();

/**
 * Positions cursor in the text-mode console.
 * @param x 1-based desired x coordinate
 * @param y 1-based desired y coordinate
 */
void gotoxy(int x, int y);

/**
 * @return horizontal cursor position
 */
int wherex();

/**
 * @return vertucal cursor position
 */
int wherey();

/**
 * Moves the cursor one block up from current position.
 */
void cursorup();

/**
 * Moves the cursor one block down from current position
 */
void cursordown();

/**
 * Moves the cursor one block right from current position
 */
void cursorfwd();

/**
 * Moves the cursor one block left from current position
 */
void cursorback();

/**
 * Suspends execution for t seconds
 * @param t number of seconds to delay
 */
void delay(int t);

/**
 * Reads a password from the console.
 * @param maskchar character to mask input echo with (usually '*').
 * @param s        pointer to a null terminated string to write the password in.
 * @param len_s    length of s
 */
void getpassword(const char maskchar, char* s, int len_s);

/**
 * Sets console text color
 * @param newcolor desired new text color
 */
void textcolor(int newcolor);

/**
 * Sets console text background color
 * @param newcolor desired new background color
 */
void textbackground(int newcolor);

/**
 * Sets console text and background colors to the defaults of the text terminal
 */
void textdefault();

/**
 * Hides cursor
 */
void cursoroff();

/**
 * Displays cursor
 */
void cursoron();

#ifndef _WIN32	// These functions are already defined in the Windows <conio.h>

/* 
 * Please note that getch(), and getche() functions behave differently on Windows,
 * and ANSI compatible systems.
 */

/**
 * Gets a chacacter from the console, but does not echo to the screen.
 */
char getch();
/**
 * Gets a character from the console and echoes to the screen.
 */
char getche();

/**
 * Determines if a key was pressed on the keyboard.
 */
int kbhit();
#endif

#endif
