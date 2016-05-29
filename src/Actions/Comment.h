#pragma once

#include "Action.h"



class Comment : public Action
{
private:
	Point Position;

public:
	Comment(ApplicationManager *pAppManager);

	virtual void ReadActionParameters();

	virtual void Execute();

};