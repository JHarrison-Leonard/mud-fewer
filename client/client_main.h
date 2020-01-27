#ifndef CLIENT_MAIN_H_
#define CLIENT_MAIN_H_
#include <ncurses.h>

//Client exit return values
#define RETURN_SUCCESS 0

//Console sizes
#define INPUT_HEIGHT 3 //Must be at least 3

//Runtime options
#define TIMEOUT 14 // 0 for non-blocking input getch, > 0 for delay blocking getch

#define WINDOW_COUNT 2
enum WindowBoxes {INPUT, CONSOLE};

//Structures
struct WindowBox
{
	WINDOW * b = nullptr; //Border
	WINDOW * c = nullptr; //Contents
};


//Functions
int main(int argc, char ** argv);

void prepInput(WindowBox& win);
void prepConsole(WindowBox& win);

void refreshFrame(WindowBox * wins, int * order, int& yLast, int& xLast);

void genBorder(WINDOW * win, const char * str);
void genBorder(WINDOW * win, std::string str);
void genBorder(WindowBox win, const char * str); //Calls on border
void genBorder(WindowBox win, std::string str);  //Calls on border

int rwmove(WINDOW * win, int y, int x);
int rwmove(WindowBox win, int y, int x); //Calls on contents

int wmovestep(WINDOW * win, int direction);
int wmovestep(WindowBox win, int direction); //Calls on contents

char isNonSpecial(int c);

//Overloaded Ncurses functions with the WindowBox structure
/* Calls on border and contents, returning the sum of the two functions */
int wrefresh(WindowBox win);
int wnoutrefresh(WindowBox win);
/* Calls on contents */
int wmove(WindowBox win, int y, int x);
int wgetch(WindowBox win);
int wdelch(WindowBox win);
int werase(WindowBox win);


#endif
