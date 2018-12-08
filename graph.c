#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/ioctl.h> // For term interaction / sending flags to it (struct winsize) 
#include "include/term.h"
#include "include/graph.h"

void resetColors() {
	fputs("\033[39m",stdout);
	fputs("\033[49m",stdout);
}

void setColor(int color) {
	fputs("\033[38;5;",stdout);
	printf("%d",color);
	fputs("m",stdout);
}

void setBgColor(int color) {
	fputs("\033[48;5;",stdout);
	printf("%d",color);
	fputs("m",stdout);
}

// Draws 2 char wide dot at current cursor placement. mode = 0 for background color, mode = 1 for '+'
void drawDot(int mode, int color) { // On graph, takes into account the contraction with cell size
	if(!mode) {
		setBgColor(color); // Set draw color 
		fputs(" ",stdout);
		fputs(" ",stdout);
		resetColors();
	} else {
		setColor(color);
		fputs("+", stdout);
		fputs("+", stdout);
		resetColors();
	}
}

// Draws a straight line. isVertical = 1 to disregard slope and raw a vertical line
void drawLine(GraphPtr graph, double slope, double xOffset, double yOffset, int isVertical, int mode, int init) { // No graph pointer cuz no edition
	// Offset is in normal unit, i is in screen unit
	int i,j;

	if(!isVertical) {
		double realHeight;
		int graphHeight;

		// First pass : draw actual points
		for(i = -graph->xSize/2 ; i < graph->xSize/2 ; i++) { // Graph size is 2x col size, but we need half of it on each side
		
			// y = f(x) computation, x and y being in graph units ; also account for zoom settings
			realHeight = slope*((1/graph->xZoom)*i+xOffset)+yOffset; // Apply x-zoom to x, add xOffset (Hz translation), then compute real y and add yOffset, 1/xZoom cause unzooming by 0.5x would mean expand what is displayed on x axis by a factor of 2
			graphHeight = (int)round(graph->yZoom*realHeight); // Apply yZoom and round to closest integer to get graph y
			
			if(abs(graphHeight) < graph->ySize && !(!init && (i == 0 || graphHeight == 0))) { // Check if the point is within graph bounds or on axes lines
				termGoto(i*2+graph->xSize,(-graphHeight)+graph->ySize); // Minus because y origin is the top of screen
				drawDot(mode,15); // Draw the point
			}
		}
		// Second pass : draw fill-up points to make lines continuous (TODO find a better way)
		// Notice this is a thing only for straight lines. Need to find a way to assess continuity so we don't end up linking +-inf if f(x) = 1/x
		// Arbitrarily start from the left to fill up holes 
		int h1, h2;
		int sign; 
		for(i = -graph->xSize/2 ; i < graph->xSize/2 ; i++) {
			h2 = (int)round(graph->yZoom*slope*((1/graph->xZoom)*i+xOffset)+yOffset); 
			h1 = (int)round(graph->yZoom*slope*((1/graph->xZoom)*(i-1)+xOffset)+yOffset); 
			if(abs(h2) < graph->ySize && abs(h1) < graph->ySize) {
				// Code for mere mortals
				if(h2 - h1 > 0) { // h2 is over h1
					for(j = h1+1 ; j < h2 ; j++) {
						termGoto(((i-1)*2)+graph->xSize, -(j*2)+graph->ySize); 
						drawDot(mode, 15);
					}
				} else if(h2 - h1 < 0) { // h2 is over h1 
					for(j = h1-1 ; j > h2 ; j--) {
						termGoto(((i-1)*2)+graph->xSize, -(j*2)+graph->ySize); 
						drawDot(mode, 15);
					}
				} // Nothing to fill if they're both at the same height

				// Code for the Chad ternary users
				/*	sign = ((h2-h1 > 0)? 1 : -1);
				for(j = h1 + sign ; sign*j < sign*h2 ; j += sign) {
					termGoto((i-1)*2, j*2); 
					drawDot(mode, 15);
				}*/
			}
		}
	} else {
		int xOffsetIntegBuf = (int)round(graph->xZoom*xOffset) + graph->xSize;
		for(int i = 0 ; i < 2*graph->ySize ; i++) {
			if(i != graph->ySize) { // Make sure we don't overwrite our beautiful axes lines
				termGoto(xOffsetIntegBuf, i);
				drawDot(mode,15);
			}
		}
	}
}

// Draw axis
GraphPtr initGraph() {
	// Getting terminal infos
	struct winsize ws = getTermSize();

	termClear();

	// Initializing graph
	Graph *graph = malloc(sizeof(Graph));
	graph->xSize = ws.ws_col/2;
	graph->ySize = ws.ws_row/2;
	graph->xZoom = 1;
	graph->yZoom = 4;

	// Draw main axes
	drawLine(graph,0,0,0,0,0,1); // x-axis
	drawLine(graph,0,0,0,1,0,1); // y-axis

	return graph;
}


void drawFct(GraphPtr graph, Fct fnction) {

}


void drawGraph(GraphPtr graph, Fct fnction) { // One function at a time ; refreshes the graph display with given function and graph settings
	// Refreshing term size on graph
	struct winsize ws = getTermSize();
	graph->xSize = ws.ws_col;
	graph->ySize = ws.ws_row;

	// Draw main axes
	drawLine(graph,0,0,0,0,0,1); // x-axis
	drawLine(graph,0,0,0,1,0,1); // y-axis
}
