#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/ioctl.h> // For term interaction / sending flags to it (struct winsize) 
#include "include/term.h"
#include "include/eval.h"
#include "include/graph.h"

// Draws graph unit sized dot at current position
// mode = 0 for background color plotting 
// mode = 1 for '+' plotting
void drawDot(int mode, int color) {
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

int *plotLine(GraphPtr graph, double slope, double xOffset, double yOffset) {
	int i;
	double realHeight;

	int *plot = malloc(graph->xSize*sizeof(int));

	// First pass : draw actual points
	for(i = 0 ; i < graph->xSize ; i++) { // Graph size is 2x col size, but we need half of it on each side

		// y = f(x) computation, x and y being in graph units ; also account for zoom settings
		// Apply xZoom to x (1/xZoom cause zooming by 0.5 actually increases the slope unlike yZooming
		// Add xOffset (horizontal translation)
		// Compute real y using the slope and add yOffset
		realHeight = slope*((1/graph->xZoom)*(i-graph->xSize/2)+xOffset)+yOffset;
		// Compute graph units, integer y(x) by applying yZoom and rounding the double to an int
		plot[i] = (int)round(graph->yZoom*realHeight);
	}

	return plot;
}


// Draws a straight line. isVertical = 1 to disregard slope and raw a vertical line
void drawLine(GraphPtr graph, int *plot, int isVertical, int mode, int init) {
	// Offset is in normal unit, i is in screen unit
	int i;

	if(!isVertical) {

		// First pass : draw actual points
		for(i = 0 ; i < graph->xSize ; i++) {
			if(abs(plot[i]) < graph->ySize && !(!init && (i == graph->xSize/2 || plot[i] == graph->ySize/2))) {
				// Using i*2 in lieu of i to account for the difference between graph and terminal units
				// Add ySize to translate screen origin to center
				termGoto(i*2+1,(-(plot[i]))+graph->ySize); // Top left is (1,1) in term coords...
				drawDot(mode,15); // Draw the point
			}
		}
		// Second pass : draw fill-up points to make lines continuous 
		// Notice this is a thing only for straight lines. Need to find a way to assess continuity so we don't end up linking +-inf if f(x) = 1/x
		// Arbitrarily start from the left to fill up holes (TODO : try to improve that, go left and right from a given point until half the way to the next)
		//int h1, h2;
		int sign; 
		for(i = 1 ; i < graph->xSize ; i++) {
			if(abs(plot[i]) < graph->ySize && abs(plot[i-1]) < graph->ySize && i != graph->xSize/2+1) { // No need to check of init since no second pass for x/y-axes */
/*				// Code for mere mortals
				if(h2 - h1 > 0) { // h2 is over h1
					for(j = h1+1 ; j < h2 ; j++) {
						if(i-1 != 0 && -j+graph->ySize != 0) {
							termGoto(((i-1)*2)+graph->xSize, -(j)+graph->ySize); 
							drawDot(mode, 42);
						}
					}
				} else if(h2 - h1 < 0) { // h2 is over h1 
					for(j = h1-1 ; j > h2 ; j--) {
						if(i-1 != 0 && -j+graph->ySize != 0) {
							termGoto(((i-1)*2)+graph->xSize, -(j)+graph->ySize); 
							drawDot(mode, 0);
						}
					}
				} // Nothing to fill if they're both at the same height
*/
				// Code for the Chad ternary users
				int j;
				sign = ((plot[i]-plot[i-1] > 0)? 1 : -1);
				for(j = plot[i-1] + sign ; sign*j < sign*plot[i] ; j += sign) {
					if(-j != 0) {
						termGoto(((i-1)*2)+1, -(j)+graph->ySize); 
						drawDot(mode, 42);
					}
				}
			}
		}
	} else {
		//int xOffsetIntegBuf = (int)round(graph->xZoom*xOffset) + graph->xSize; // no more xOffset
		for(int i = 0 ; i < 2*graph->ySize ; i++) {
			if(i != graph->ySize) { // Make sure we don't overwrite our wunderbar axes lines
				termGoto(graph->xSize, i);
				drawDot(mode,15);
			}
		}
	}
}

// Init graph with default values and plot x and y axis
GraphPtr initGraph() {
	// Getting terminal infos
	struct winsize ws = getTermSize();
	
	// Wipe the slate clean
	termClear();

	// Initialize graph
	Graph *graph = malloc(sizeof(Graph));
	graph->xSize = ws.ws_col/2 + 1;
	graph->ySize = ws.ws_row/2 + 1;
	graph->xZoom = 1;
	graph->yZoom = 1; // TODO

	// Draw main axes
	int *xAxis = plotLine(graph, 0, 0, 0);
	drawLine(graph,xAxis,0,0,1); // x-axis
	drawLine(graph,NULL,1,0,1); // y-axis, no plot for invalid vertical function

	return graph;
}


void drawFct(GraphPtr graph, Fct fnction) {

}

// Refreshes the graph display with given function and graph settings
void drawGraph(GraphPtr graph, Fct fnction) { 
	// Refreshing term size on graph
	struct winsize ws = getTermSize();
	graph->xSize = ws.ws_col;
	graph->ySize = ws.ws_row;

	// Draw main axes
	//drawLine(graph,0,0,0,0,0,1); // x-axis
	//drawLine(graph,0,0,0,1,0,1); // y-axis
}
