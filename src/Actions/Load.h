#pragma once

#include "Action.h"
#include <fstream>



class Load : public Action
{
private:
	Point Position;
public:
	Load(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};