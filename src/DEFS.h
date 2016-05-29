#ifndef DEFS_H
#define DEFS_H

//This file contais some global constants and definitions to be used in the project.

enum ActionType //The actions supported (you can add more if needed)
{
	START,
	END,
	READ,
	WRITE,

	ADD_ASSIGN,
	ADD_SMPL_ASSIGN, //Add simple assignment statement
	ADD_VAR_ASSIGN,	//Add variable assignment statement
	ADD_SO_ASSIGN,  // Add single operator assignment statement

	ADD_CONDITION, //Add a conditional statement (for if and while-loop statements)

	ADD_CONNECTOR, //Add a connector between two statements

	COMMENT,

	EDIT_STAT,  //Edit a statement

	EDIT_CONNECTOR,

	SELECT,		//Select a statement, a connector
	MULT_SELET,

	COPY,
	CUT,
	PASTE,

	DEL,		//Delete a figure(s)
	MOVE,		//Move a figure(s)
	RESIZE,		//Resize a figure(s)

	ZOOM_IN,
	ZOOM_OUT,

	UNDO,
	REDO,

	SAVE,		//Save the whole graph to a file
	LOAD,		//Load a graph from a file
	MINIMIZE,	//Minimize the window
	EXIT,		//Exit the application

	VALIDATE,
	RUN,
	DEBUG,

	GENERATE_CODE,

	STATUS,		//A click on the status bar or on an empty place in a toolbar

	DSN_MODE, //Switch to Design mode
	SIM_MODE	//Switch to simulatiom mode
};

enum  MODE	//Modes of operation
{
	DESIGN,
	SIMULATION
};

#ifndef NULL
#define NULL 0
#endif

#endif