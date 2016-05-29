#pragma once

#include "Action.h"
#include "Validate.h"

class Run : public Action
{
public:
	Run(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();
};