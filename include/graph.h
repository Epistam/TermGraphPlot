#ifndef GRAPH_H
#define GRAPH_H

/********************
 * Graph management *
 ********************/

#define GRAPH_AXIS_COLOR 15

typedef struct {
	int xSize; // x Sizes will be double those of a normal terminal : we want square units
	int ySize;
	double xZoom;
	double yZoom;
	int zoomMode; // 0 for linked, 1 for hz only, 2 for vert only. Zoom with arrows, change mode using menu or keybind TODO
} Graph, *GraphPtr;

// Draws graph unit sized dot at current position
// mode = 0 for background color plotting 
// mode = 1 for '+' plotting
void drawDot(int mode, int color);

// Function plotting
// Draw a line. Hopefully, soon transfer regular lines with slopes to drawFct and keep axis drawing only here, and take care of the x = a case  TODO
// mode is same as drawDot
void drawLine(GraphPtr graph, double slope, double xOffset, double yOffset, int isVertical, int mode, int init); 
void drawFct(GraphPtr graph, Fct fnction); 

// Global graph routines
GraphPtr initGraph(); // Init graph with default values and plot x and y axis
void drawGraph(GraphPtr graph, Fct fnction); // Refreshes the graph display with given function and graph settings

#endif
