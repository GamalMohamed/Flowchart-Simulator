#pragma once

#include "Action.h"


class EditStat : public Action
{
private:
	Point Position;

public:
	EditStat(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};