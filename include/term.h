#ifndef TERM_H
#define TERM_H

void termClear();// move to (0,0) too
void termClearLine(); // move to (0,0) too
void termUp();
void termDown();
void termFwd();
void termBack();
void termGoto(int x, int y); // some fucking how coords are inverted in the term codes

void initTerm();
void resetTerm();
struct winsize getTermSize();

#endif 
