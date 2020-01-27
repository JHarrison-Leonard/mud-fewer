#include <ncurses.h>
#include <string>
#include "client_main.h"


int main(int argc, char ** argv)
{
	//Variable definitions
	bool completelyDone = false;
	int yScrPre, xScrPre;
	//Windows
	WindowBox wb[WINDOW_COUNT];
	int order[WINDOW_COUNT];
	//Input drivers
	std::string inputStr = "";
	//Console drivers
	/* Some class that handles output history */
	
	//Read in user config
	/*fuk i dunno*/
	
	//Initialize ncurses screen
	initscr();  //Init screen allocate memory and all that
	noecho();   //Deactivates auto echoing of all input
	
	//Prepare windows
	prepInput(wb[INPUT]);     order[0] = INPUT;
	prepConsole(wb[CONSOLE]); order[1] = CONSOLE;
	refresh(); for(int i = 0; i < WINDOW_COUNT; i++) wrefresh(wb[i]);
	getmaxyx(stdscr, yScrPre, xScrPre);
	wmove(wb[INPUT], 0, 0);
	
	
	//Do things
	while(!completelyDone)
	{
		refreshFrame(wb, order, yScrPre, xScrPre);
		
		int ch = wgetch(wb[INPUT]);
		if(ch != ERR)
		{
			if(ch == '\n')
				completelyDone = true;
		}
	}
	
	
	//Clean memory
	endwin(); //Kill ncurses screen
	
	//Default return
	return RETURN_SUCCESS;
}


char isNonSpecial(int c)
{
	//All readable ASCII characters
	if(' ' <= c && c <= '~')
		return (char)c;
	
	//Default return
	return 0;
}

int rwmove(WINDOW * win, int y, int x)
{
	int cy, cx, yMax, xMax, ny, nx;
	getyx(win, cy, cx);
	getmaxyx(win, yMax, xMax);
	
	if(cy + y < 0)
		ny = 0;
	else if(cy + y > yMax)
		ny = yMax;
	else
		ny = cy + y;
	
	if(cx + x < 0)
		nx = 0;
	else if(cx + x > xMax)
		nx = xMax;
	else
		nx = cx + x;
	
	//Default return
	return wmove(win, ny, nx);
}
int rwmove(WindowBox win, int y, int x)
{	return rwmove(win.c, y, x);  }

int wmovestep(WINDOW * win, int direction)
{
	switch(direction)
	{
		case KEY_UP:
			return rwmove(win, -1, 0);
		case KEY_DOWN:
			return rwmove(win, 1, 0);
		case KEY_LEFT:
			return rwmove(win, 0, -1);
		case KEY_RIGHT:
			return rwmove(win, 0, 1);
	}
	
	//Default return
	return ERR;
}
int wmovestep(WindowBox win, int direction)
{	return wmovestep(win.c, direction);  }

void prepInput(WindowBox& win)
{
	//Get y and x maximums of terminal
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	
	win.b = newwin(INPUT_HEIGHT, xMax, yMax - INPUT_HEIGHT, 0);
	win.c = newwin(INPUT_HEIGHT - 2, xMax - 2, yMax - INPUT_HEIGHT + 1, 1);
	
	wtimeout(win.c, TIMEOUT);
	keypad(win.c, true);
	
	genBorder(win, "Input");
	
	//Default return
	return;
}

void prepConsole(WindowBox& win)
{
	//get y and x maximums of terminal
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	
	win.b = newwin(yMax - INPUT_HEIGHT, xMax, 0, 0);
	win.c = newwin(yMax - INPUT_HEIGHT - 2, xMax - 2, 1, 1);
	
	genBorder(win, "Console");
	
	//Default return
	return;
}

void refreshFrame(WindowBox * wins, int * order, int& yLast, int& xLast)
{
	int yScr, xScr;
	getmaxyx(stdscr, yScr, xScr);
	bool resize = (yScr == yLast) && (xScr == xLast);
	
	for(int i = WINDOW_COUNT; i >= 0; i--)
	{
		switch(order[i])
		{
			case INPUT:
				if(resize)
				{
					wresize(wins[INPUT].b, INPUT_HEIGHT, xScr);
					wresize(wins[INPUT].c, INPUT_HEIGHT - 2, xScr - 2);
					mvwin(wins[INPUT].b, yScr - INPUT_HEIGHT, 0);
					mvwin(wins[INPUT].c, yScr - INPUT_HEIGHT + 1, 1);
					genBorder(wins[INPUT], "Input");
				}
				werase(wins[INPUT]);
				wnoutrefresh(wins[INPUT]);
				break;
			
			case CONSOLE:
				if(resize)
				{
					wresize(wins[CONSOLE].b, yScr - INPUT_HEIGHT, xScr);
					wresize(wins[CONSOLE].c, yScr - INPUT_HEIGHT - 2, xScr - 2);
					mvwin(wins[CONSOLE].b, 0, 0);
					mvwin(wins[CONSOLE].c, 1, 1);
					genBorder(wins[CONSOLE], "Console");
				}
				werase(wins[CONSOLE]);
				mvwprintw(wins[CONSOLE].c, 0, 0, "TEST\nINFORMATION\nPLEASE\nIGNORE\nDAKKADAKKADAKKADAKKADAKKADAKKADAKKA\n");
				wnoutrefresh(wins[CONSOLE]);
				break;
		}
	}
	doupdate();
	
	yLast = yScr;
	xLast = xScr;
	
	//Default return
	return;
}

void genBorder(WINDOW * win, const char * str)
{
	box(win, 0, 0);
	mvwprintw(win, 0, 1, "%s", str);
	
	//Default return
	return;
}
void genBorder(WINDOW * win, std::string str)
{	return genBorder(win, str.c_str());  }
void genBorder(WindowBox win, const char * str)
{	return genBorder(win.b, str);  }
void genBorder(WindowBox win, std::string str)
{	return genBorder(win.b, str.c_str());  }

int wrefresh(WindowBox win)
{	return wrefresh(win.b) + wrefresh(win.c);  }

int wnoutrefresh(WindowBox win)
{	return wnoutrefresh(win.b) + wnoutrefresh(win.c);  }

int wmove(WindowBox win, int y, int x)
{	return wmove(win.c, y, x);  }

int wgetch(WindowBox win)
{	return wgetch(win.c);  }

int wdelch(WindowBox win)
{	return wdelch(win.c);  }

int werase(WindowBox win)
{	return werase(win.c);  }
