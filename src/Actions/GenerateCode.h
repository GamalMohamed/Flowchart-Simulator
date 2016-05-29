#pragma once

#include "Action.h"
#include "Validate.h"

class GenerateCode : public Action
{
public:
	GenerateCode(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();
};