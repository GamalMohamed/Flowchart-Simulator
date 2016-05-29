#pragma once
#include "Action.h"
#include "..\Statements\Connector.h"

class EditConnector : public Action
{

public:
	EditConnector(ApplicationManager *pAppManager);

	//Read statement position
	virtual void ReadActionParameters();

	//Create and add a statement to the list of statements
	virtual void Execute();

};