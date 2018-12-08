#ifndef GRAPH_H
#define GRAPH_H

#define GRAPH_AXIS_COLOR 15

typedef struct {
	int xSize; // In chars, 1/2 the term size cuz we want square units
	int ySize;
	double xZoom;
	double yZoom;
	int zoomMode; // 0 for linked, 1 for hz only, 2 for vert only. Zoom with arrows, use 'z' to change mode
} Graph, *GraphPtr;

typedef struct {
	int a; // Placeholder
} Fct;

// Term color related subroutines
void resetColors();
void setColor(int color); // color is the 255 colors mode bash code 
void setBgColor(int color); // idem

// Draw graph unit sized dot
void drawDot(int mode, int color); // Draw graph unit sized dot at current position : mode = 0 for background color (x / y-axis), mode = 1 for plotting (with '+') + idem
void drawLine(GraphPtr graph, double slope, double xOffset, double yOffset, int isVertical, int mode, int init); // Draws a line. Settings are self explanatory, mode is the same as in drawDot
void drawFct(GraphPtr graph, Fct fnction); // TODO : merge drawLine in drawFct

GraphPtr initGraph();
void drawGraph(GraphPtr graph, Fct fnction); // One function at a time ; refreshes the graph display with given function and graph settings

#endif
