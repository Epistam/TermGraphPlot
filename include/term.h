#ifndef TERM_H
#define TERM_H

/********************************
 * Terminal function / routines *
 ********************************/

// Whole terminal / screen rountines
void termClear(); // Clears terminal and then moves to (0,0)
struct winsize getTermSize(); // Gets terminal size in characters and returns a struct defined in ioctl.h 
void initTerm(); // Initializes terminal for use in the program
void resetTerm(); // Restores terminal to its previous settings for nominal console use after execution

// Single line
void termClearLine(); // Clear a whole line and go back to (0,0) (or line beginning ? can't remember, don't use this one anyway)

// Cursor movement
void termUp(); // Moves the cursor up by 1
void termDown(); // idem but down
void termFwd(); // idem but forward
void termBack(); // idem but backwards
void termGoto(int x, int y); // Puts the cursor at specified coordinates (origin is at the top left of the screen)

// Cursor color
void resetColors(); // Reset colors to default ones
void setColor(int color); // Set character color. Use bash codes for a 256 colors setting.
void setBgColor(int color); // idem with background colors

#endif 
