#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
static WINDOW *screen;

int main(void)
{
	/* Init curses */
	int rc=0;
        int key,i;
	screen=initscr();
	noecho();
	keypad(stdscr, 1);
	raw();
	cbreak();
	key=getch();
	rc=0;
	if ( key != '1' ) rc=1;
	endwin();
	return (rc);
	
}
