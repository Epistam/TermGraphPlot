#include <stdio.h>
#include <stdlib.h>
#include <termios.h> // Terminal attributes
#include <unistd.h> // For STDIN_FILENO
#include <sys/ioctl.h> // For term interaction / sending flags to it 
#include "include/term.h"

struct termios orig_termios;

void termClear() {printf("\033[2J");} // move to (0,0) too
void termClearLine() { printf("\033[K");} // move to (0,0) too
void termUp() { printf("\033\[1A");}
void termDown() { printf("\033\[1B");}
void termFwd() { printf("\033\[1C");}
void termBack() { printf("\033\[1D");}
void termGoto(int x, int y) { printf("\033[%d;%dH", y, x);} // some fucking how coords are inverted in the term codes

void initTerm() {
	// Get current terminal attributes
	tcgetattr(STDIN_FILENO, &orig_termios);
	// Disable raw mode at exit
	atexit(resetTerm);
	// Duplicate attributes
	struct termios raw = orig_termios;
	// Disable echo and canonical on the duplicate attributes
	raw.c_lflag &= ~(ECHO | ICANON);
	// "commit" new attributes to shell
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void resetTerm() {
	// Resend original attributes to terminal
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	// Restoring cursor
	fputs("\033[?25h",stdout);
}

struct winsize getTermSize() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w;
}
