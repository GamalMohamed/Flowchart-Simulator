#pragma once

#include "Action.h"
#include <fstream>



class Save : public Action
{
private:
	Point Position;	
public:
	Save(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};