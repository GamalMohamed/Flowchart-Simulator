#pragma once

#include "Action.h"
#include "Validate.h"

class Debug : public Action
{
public:
	Debug(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();
};