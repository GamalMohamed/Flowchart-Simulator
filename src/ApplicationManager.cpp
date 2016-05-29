#include "ApplicationManager.h"
#include"Statements\Connector.h"

//Constructor
ApplicationManager::ApplicationManager()
{
	//Create Input and output
	pOut = new Output(this);
	pIn = pOut->CreateInput();

	StatCount = ConnCount = SelectedCount = ConnSelectedCount = nE = nS = 0;
	pSelectedStat = NULL;	//no Statement is selected yet
	ZM = false;


	//Create an array of Statement pointers and set them to NULL		
	for (int i = 0; i<MaxCount; i++)
	{
		StatList[i] = NULL;
		ConnList[i] = NULL;
		SelectedList[i] = NULL;
		ConnSelectedList[i] = NULL;
	}
}

//==================================================================================//
//								Actions Related Functions							//
//==================================================================================//
ActionType ApplicationManager::GetUserAction() 
{
	//Ask the input to get the action from the user
	return pIn->GetUserAction(p.x,p.y);	
}

////////////////////////////////////////////////////////////////////////////////////
//Creates an action and executes it
void ApplicationManager::ExecuteAction(ActionType ActType)
{
	Action* pAct = NULL;

	//According to ActionType, create the corresponding action object
	switch (ActType)
	{
		case EXIT:
			return;

		case MINIMIZE:
			ShowWindow(pOut->GetWind()->GetHWind(), SW_MINIMIZE); //Minimize the window
			return;

		case STATUS:
			return;

		case DSN_MODE:
			pOut->ClearAll();
			pOut->CreateDesignToolBar();
			break;

		case SIM_MODE:
			if (!ZM)
			{
				pOut->ClearAll();
				pOut->CreateSimulationToolBar();
			}
			else
				pOut->MsgBox("You Should Zoom out first before switching to Simulation mode !!", "Info", false);
			break;

		case SELECT:
			pAct = new Select(this);
			break;

		case START:
			if (nS<1)
				pAct = new AddStart(this);
			else
				pOut->MsgBox("You can't add more than one Start Statement !!", "Info", false);

			break;
		case END:
			if (nE<1)
				pAct = new AddEnd(this);
			else
				pOut->MsgBox("You can't add more than one End Statement !!", "Info", false);

			break;
		case READ:
			pAct = new AddRead(this);
			break;
		case WRITE:
			pAct = new AddWrite(this);
			break;

		case ADD_ASSIGN:
			pAct = new AddAssign(this);
			break;

		case ADD_CONDITION:
			pAct = new AddCond(this);
			break;

		case COMMENT:
			pAct = new Comment(this);
			break;

		case EDIT_STAT:
			pAct = new EditStat(this);
			break;

		case DEL:
			if (SelectedCount > 0 || ConnSelectedCount > 0)
				pAct = new Delete(this);
			else
				pOut->MsgBox("Nothing selected to be deleted !! You Should select the item(s) first then click on the action", "Info", false);
			break;

		case COPY:
			if (SelectedCount>0)
				pAct = new Copy(this);
			else
				pOut->MsgBox("Nothing selected to be Copied !! You Should select the item(s) first then click on the action", "Info", false);
			break;

		case CUT:
			if (SelectedCount>0)
				pAct = new Cut(this);
			else
				pOut->MsgBox("Nothing selected to be Cut !! You Should select the item(s) first then click on the action", "Info", false);
			break;

		case PASTE:
		    pAct = new Paste(this);
			break;

		case MOVE:
			if (SelectedCount>0)
				pAct = new Move(this);
			else
				pOut->MsgBox("Nothing selected to be Moved !! You Should select the item(s) first then click on the action", "Info", false);
			break;

		case ADD_CONNECTOR:
			pAct = new AddConnector(this);
			break;

		case EDIT_CONNECTOR:
			pAct = new EditConnector(this);
			break;
		
		case SAVE:
			pAct = new Save(this);
			break;

		case LOAD:
			pAct = new Load(this);
			break;

		case VALIDATE:
			pAct = new Validate(this);
			break;

		case RUN:
			pAct = new Run(this);
			break;

		case DEBUG:
			pAct = new Debug(this);
			break;

		case GENERATE_CODE:
			pAct = new GenerateCode(this);
			break;

		case ZOOM_IN:
			if (SelectedCount>0)
				pAct = new ZoomIN(this);
			else
				pOut->MsgBox("Nothing selected to be Zoomed to !! You Should select the item(s) first then click on this action", "Info", false);
			break;

		case ZOOM_OUT:
			if (ZM)
				pAct = new ZoomOUT(this);
			else
				pOut->MsgBox("Nothing to zoom out from !!", "Info", false);
			break;

		case UNDO:
			pOut->MsgBox("Not implemented.", "Info", false);
			return;

		case REDO:
			pOut->MsgBox("Not implemented.", "Info", false);
			return;
	}
	
	//Execute the created action
	if(pAct != NULL)
	{
		pAct->Execute();//Execute
		delete pAct;	//Action is not needed any more ==> delete it
	}
}



//==================================================================================//
//						Statements Management Functions								//
//==================================================================================//
//Add a statement to the list of statements
void ApplicationManager::AddStatement(Statement *pStat, bool L)
{
	if (StatCount < MaxCount)
	{
		StatList[StatCount++] = pStat;
		if (!L)
			pStat->setID(StatCount);

		if (dynamic_cast<Start*>(pStat))
			nS++;
		if (dynamic_cast<End*>(pStat))
			nE++;
	}
	else
		pOut->MsgBox("You can't add more than" + to_string(MaxCount) + "Statements !!", "Info", false);
}

//Add a connector to the list of connectors
void ApplicationManager::AddConn(Connector *pConn)
{
	if (ConnCount < MaxCount)
	{
		ConnList[ConnCount++] = pConn;
	}
	else
		pOut->MsgBox("You can't add more than" + to_string(MaxCount) + "Connectors !!", "Info", false);
}

////////////////////////////////////////////////////////////////////////////////////
Statement *ApplicationManager::GetStatement(Point Position) const
{
	//If this point P(x,y) belongs to a statement return a pointer to it.Otherwise, return NULL
	//Add your code here to search for a statement given a point P(x,y)	

	for (int i = 0; i < StatCount; i++)
	{
		Point p = StatList[i]->getPosition();

		if (dynamic_cast<Start*> (StatList[i]))
		{
			if (((Position.x >= p.x - 0.5*StatList[i]->getWidth()) && (Position.x <= p.x + 0.5*StatList[i]->getWidth())) && ((Position.y >= p.y) && (Position.y <= p.y + StatList[i]->getHeight())))
				return StatList[i];
		}

		else if (dynamic_cast<End*> (StatList[i]))
		{
			if (((Position.x >= p.x - 0.5*StatList[i]->getWidth()) && (Position.x <= p.x + 0.5*StatList[i]->getWidth())) && ((Position.y >= p.y) && (Position.y <= p.y + StatList[i]->getHeight())))
				return StatList[i];
		}

		else if (dynamic_cast<Assign*> (StatList[i]))
		{
			if (((Position.x >= p.x - 0.5*StatList[i]->getWidth()) && (Position.x <= p.x + 0.5*StatList[i]->getWidth())) && ((Position.y >= p.y) && (Position.y <= p.y + StatList[i]->getHeight())))
				return StatList[i];
		}

		else if (dynamic_cast<Read*> (StatList[i]))
		{
			if (((Position.x >= p.x - 0.75*StatList[i]->getWidth()) && (Position.x <= p.x + 0.5*StatList[i]->getWidth())) && ((Position.y >= p.y) && (Position.y <= p.y + StatList[i]->getHeight())))
				return StatList[i];
		}

		else if (dynamic_cast<Write*> (StatList[i]))
		{
			if (((Position.x >= p.x - 0.75*StatList[i]->getWidth()) && (Position.x <= p.x + 0.5*StatList[i]->getWidth())) && ((Position.y >= p.y) && (Position.y <= p.y + StatList[i]->getHeight())))
				return StatList[i];
		}

		else if (dynamic_cast<Cond*> (StatList[i]))
		{
			if (Position.y >= p.y  && Position.x <= p.x + StatList[i]->getWidth() / 2 && Position.x >= p.x - StatList[i]->getWidth() / 2 && Position.y <= p.y + StatList[i]->getHeight())
				return StatList[i];
		}
	}

	return NULL;
}

Connector* ApplicationManager::GetConnector(Point Position) const
{
	//If this point P(x,y) belongs to a connector return a pointer to it. Otherwise, return NULL
	if (GetStatement(Position) == NULL)
	{
		color c = pOut->GetColor(Position);
		if (pOut->isDrawColor(c))
		{
			for (int i = 0; i < ConnCount; ++i)
			{
				pair<Point, Point> Segments[1000];
				ConnList[i]->GetSegments(Segments);
				int SegCount = ConnList[i]->GetSegCount();
				for (int j = 0; j < SegCount; ++j)
				{
					int x1 = Segments[j].first.x;
					int y1 = Segments[j].first.y;
					int x2 = Segments[j].second.x;
					int y2 = Segments[j].second.y;
					if (abs(Position.x - x1) <= 4 && abs(Position.x - x2) <= 4 && Position.y >= min(y1, y2) && Position.y <= max(y1, y2)
						|| abs(Position.y - y1) <= 4 && abs(Position.y - y2) <= 4 && Position.x >= min(x1, x2) && Position.x <= max(x1, x2))
					{
						return ConnList[i];
					}
				}
			}
		}
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
//Returns the selected statement
Statement *ApplicationManager::GetSelectedStatement() const
{	return pSelectedStat;	}

////////////////////////////////////////////////////////////////////////////////////
//Set the statement selected by the user
void ApplicationManager::SetSelectedStatement(Statement *pStat)
{	pSelectedStat = pStat;	}



int  ApplicationManager::getStatCount()
{ 
	return StatCount;
}

int  ApplicationManager::getConnCount()
{
	return ConnCount;
}

int ApplicationManager::getSelectedCount()
{
	return SelectedCount;
}

int ApplicationManager::getConnSelectedCount()
{
	return ConnSelectedCount;
}

Statement** ApplicationManager::GetAllSelected() const
{
	return SelectedList;
}

Connector** ApplicationManager::GetAllSelectedConn() const
{
	return ConnSelectedList;
}

void  ApplicationManager::RemoveSelected(Statement* pStat)
{
	int loc = 0;
	for (int i = 0; i < SelectedCount; i++)
	{
		if (pStat == SelectedList[i])
		{
			loc = i;
			break;
		}
	}

	for (int j = loc; j < SelectedCount; j++)
			SelectedList[j] = SelectedList[j + 1];
	SelectedCount--;
}

void ApplicationManager::EmptyStatList()
{
	for (int i = 0; i < StatCount; i++)
		StatList[i] = NULL;
	StatCount = 0;
}

void ApplicationManager::EmptyConnList()
{
	for (int i = 0; i < ConnCount; i++)
		ConnList[i] = NULL;
	ConnCount = 0;
}

void ApplicationManager::EmptySelectedList()
{
	for (int i = 0; i < SelectedCount; i++)
		SelectedList[i] = NULL;
	SelectedCount = 0;
}

void  ApplicationManager::AddSelected(Statement* pStat)
{
	if (SelectedCount < MaxCount)
		SelectedList[SelectedCount++] = pStat;
}

void  ApplicationManager::RemoveSelectedConn(Connector* pConn)
{
	int loc = 0;
	for (int i = 0; i < ConnSelectedCount; i++)
	{
		if (pConn == ConnSelectedList[i])
		{
			loc = i;
			break;
		}
	}

	for (int j = loc; j < ConnSelectedCount; j++)
		ConnSelectedList[j] = ConnSelectedList[j + 1];
	ConnSelectedCount--;
}

void ApplicationManager::EmptyConnSelectedList()
{
	for (int i = 0; i < ConnSelectedCount; i++)
		ConnSelectedList[i] = NULL;
	ConnSelectedCount = 0;
}

void  ApplicationManager::AddSelectedConn(Connector* pConn)
{
	if (ConnSelectedCount < MaxCount)
		ConnSelectedList[ConnSelectedCount++] = pConn;
}


void ApplicationManager::setZoomMode(bool z)
{
	ZM = z;
}

bool  ApplicationManager::getZoomMode()
{
	return ZM;
}

//==================================================================================//
//							Interface Management Functions							//
//==================================================================================//

//Draw all figures on the user interface
void ApplicationManager::UpdateInterface() const
{
	if (UI.AppMode == DESIGN)
	{
		pOut->CreateDesignToolBar();
		pOut->ClearDrawArea();
	}
	else
		pOut->CreateSimulationToolBar();

	//Draw all Statements
	for (int i = 0; i < StatCount; i++)
	{
		if (ZM)
		{
			if (StatList[i]->getZoomed())
			{
				if (!(StatList[i]->getState() == "Cut" || StatList[i]->getState() == "Copy"))
					StatList[i]->Draw(pOut);
			}
		}
		else
		{
			if (!(StatList[i]->getState() == "Cut" || StatList[i]->getState() == "Copy"))
				StatList[i]->Draw(pOut);
		}
	}

	// Draw all Connectors
	if (!ZM)
	{
		for (int i = 0; i<ConnCount; i++)
			ConnList[i]->Draw(pOut);
	}
}

////////////////////////////////////////////////////////////////////////////////////
//Return a pointer to the input
Input *ApplicationManager::GetInput() const
{	return pIn; }

//Return a pointer to the output
Output *ApplicationManager::GetOutput() const
{	return pOut; }
////////////////////////////////////////////////////////////////////////////////////


//Destructor
ApplicationManager::~ApplicationManager()
{
	for (int i = 0; i < StatCount; i++)
		delete StatList[i];
	for (int i = 0; i < ConnCount; i++)
		delete ConnList[i];
	for (int i = 0; i < SelectedCount; i++)
		delete SelectedList[i];
	for (int i = 0; i < ConnSelectedCount; i++)
		delete ConnSelectedList[i];
	delete pIn;
	delete pOut;
}