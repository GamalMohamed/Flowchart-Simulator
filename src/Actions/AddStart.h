#pragma once
#include "Action.h"
#include "..\Statements\Start.h"

//Add Start Statement Action
//This class is responsible for 
// 1 - Getting  stat. coordinates from the user
// 2 - Creating an object of this class and fill it parameters
// 3 - Adding the created object to the list of statements

class AddStart : public Action
{
private:
	Point Position;	//Position where the user clicks to add the stat.

public:
	AddStart(ApplicationManager *pAppManager);

	//Read statement position
	virtual void ReadActionParameters();

	//Create and add a statement to the list of statements
	virtual void Execute();

};