#pragma once

#include "Action.h"


class Move : public Action
{
private:
	Point Position;	//Position where the user clicks to add the stat.
public:
	Move(ApplicationManager *pAppManager);

	
	virtual void ReadActionParameters();

	
	virtual void Execute();

};