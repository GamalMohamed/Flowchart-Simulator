#ifndef OUPTUT_H
#define OUPTUT_H

#include "Input.h"
class ApplicationManager; //forward class declaration


class Output	//The application manager should have a pointer to this class
{
protected:

	//Pointer to ApplicationManager because output needs this pointer for its functions
	ApplicationManager *pManager;

private:	
	window* pWind;	//Pointer to the Graphics Window
public:
	Output(ApplicationManager*);

	window* CreateWind(int, int, int , int);
	void CreateDesignToolBar();	//Tool bar of the design mode
	void CreateSimulationToolBar();//Tool bar of the simulation mode

	void CreateStatusBar();

	Input* CreateInput(); //creates a pointer to the Input object	
	void ClearStatusBar();	//Clears the status bar
	void ClearDrawArea();	//Clears the drawing area

	// -- Statements Drawing Functions

	void DrawStart(Point TopCenter, int width, int height, string text, bool Selected = false, bool Z = false);

	void DrawEnd(Point TopCenter, int width, int height, string text, bool Selected = false, bool Z = false);

	void DrawAssign(Point TopCenter, int &width, int &height, string Text, bool Selected = false, bool Z = false);

	void DrawRead(Point TopCenter, int &width, int &height, string Text, bool Selected = false, bool Z = false);

	void DrawWrite(Point TopCenter, int &width, int &height, string Text, bool Selected = false, bool Z = false);

	void DrawCondtional(Point TopCenter, int &width, int &height, string Text, bool Selected = false, bool Z = false);

	void DrawConnector(Point From, Point To, pair<Point, Point> Segments[], int &SegCount, bool Clear = false, bool Selected = false);

	void ClearAll();

	void ClearStat(Point, char, int, int, bool = false);

	void PrintMessage(string msg);	//Prints a message on Status bar
	void MsgBox(string message, string caption, bool isError = false) const;

	color GetColor(Point) const;

	bool isDrawColor(color) const;

	window* GetWind() const { return pWind; };

	void CreateMainMenu(HWND hWnd);

	~Output();
};

#endif