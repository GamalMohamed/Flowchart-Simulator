#pragma once

#include "Action.h"


class Paste : public Action
{
public:
	Paste(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};