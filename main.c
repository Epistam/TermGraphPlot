#include <stdio.h>
#include <stdlib.h>
#include <termios.h> // Terminal attributes
#include <unistd.h> // For STDIN_FILENO
#include <sys/ioctl.h> // For term interaction / sending flags to it 
#include "include/term.h"
#include "include/eval.h"
#include "include/graph.h"

int main(void) {
		
	initTerm();
	Graph *graph = initGraph();

	//drawLine(graph,1,0,0,0,1,0); // x-axis
	
	int *line = plotLine(graph, 1, 0, 0);
	drawLine(graph,line,0,1,0);

	resetTerm();

	return EXIT_SUCCESS;
}
