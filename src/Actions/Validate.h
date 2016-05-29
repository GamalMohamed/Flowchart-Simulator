#pragma once

#include "Action.h"


class Validate : public Action
{
public:
	Validate(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

	bool isValid(string& err);
};