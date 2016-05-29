#pragma once

#include "Action.h"


class Delete : public Action
{
private:
	Point Position;

public:
	Delete(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};