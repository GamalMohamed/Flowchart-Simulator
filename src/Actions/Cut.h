#pragma once

#include "Action.h"


class Cut : public Action
{
public:
	Cut(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};