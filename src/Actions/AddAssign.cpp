#include "AddAssign.h"



#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

//#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
AddAssign::AddAssign(ApplicationManager *pAppManager):Action(pAppManager)
{}

void AddAssign::ReadActionParameters()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();

	pOut->PrintMessage("Value Assignment Statement: Click to add the statement");

	while (1)
	{
		pIn->GetPointClicked(Position);

		if (Position.y > UI.TlBrWdth && Position.y < UI.height - UI.StBrWdth)
		{
			//Calculating each corner of the statement block then check if it's inside another statement
			Point C1(Position.x - UI.ASSGN_WDTH / 2, Position.y);
			Point C2(Position.x + UI.ASSGN_WDTH / 2, Position.y);
			Point C3(Position.x - UI.ASSGN_WDTH / 2, Position.y + UI.ASSGN_HI);
			Point C4(Position.x + UI.ASSGN_WDTH / 2, Position.y + UI.ASSGN_HI);

			if (pManager->GetStatement(C1) == NULL && pManager->GetStatement(C2) == NULL && pManager->GetStatement(C3) == NULL && pManager->GetStatement(C4) == NULL)
			{
				pOut->ClearStatusBar();
				break;
			}
			else
				pOut->PrintMessage("Can't add statement near another one !! Click in another valid position");
		}
		else
			pOut->PrintMessage("Invalid area !! Click in another valid position");
	}
}

void AddAssign::Execute()
{
	this->ReadActionParameters();
	int c = 0;
	while (1)
	{
		//Calculating left corner of assignement statement block
		//Point Corner;
		//Corner.x = Position.x - UI.ASSGN_WDTH/2;
		//Corner.y = Position.y ;
		Assign *pAssign = new Assign(Position);

		Output *pOut = pManager->GetOutput();
		pOut->PrintMessage("Enter String: ");

		Input *pIn = pManager->GetInput();
		bool y = pAssign->setData((pIn->GetString(pOut)));

		if (y)
		{
			pManager->AddStatement(pAssign);
			pOut->ClearStatusBar();
			break;
		}
		else
		{
			pOut->MsgBox("Invalid text !! Click ok to Re-enter data", "Error !", true);
			c++;
		}

		if (c>5)
		{
			pOut->MsgBox("You entered Invalid text many times.So,Program will terminate now", "Error !", true);
			exit(1);
		}

	}
}

