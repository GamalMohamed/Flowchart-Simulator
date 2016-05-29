#pragma once

#include "Action.h"



class ZoomIN : public Action
{
private:
	Point Position;	//Position where the user clicks to add the stat.

public:
	ZoomIN(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};