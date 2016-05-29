#pragma once

#include "Action.h"



class ZoomOUT : public Action
{
private:
	Point Position;	//Position where the user clicks to add the stat.

public:
	ZoomOUT(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};