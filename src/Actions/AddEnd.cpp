#include "AddEnd.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

//#include <sstream>

using namespace std;

AddEnd::AddEnd(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void AddEnd::ReadActionParameters()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();

	pOut->PrintMessage("End Statement: Click to add the statement");

	while (1)
	{
		pIn->GetPointClicked(Position);

		if (Position.y > UI.TlBrWdth && Position.y < UI.height - UI.StBrWdth)
		{
			//Calculating each corner of the statement block then check if it's inside another statement
			Point C1(Position.x - UI.END_WDTH / 2, Position.y);
			Point C2(Position.x + UI.END_WDTH / 2, Position.y);
			Point C3(Position.x - UI.END_WDTH / 2, Position.y + UI.END_HI);
			Point C4(Position.x + UI.END_WDTH / 2, Position.y + UI.END_HI);

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

void AddEnd::Execute()
{
	this->ReadActionParameters();

	//Calculating left corner of the statement block
	//Point Corner;
	//Corner.x = Position.x - UI.ASSGN_WDTH / 2;
	//Corner.y = Position.y;

	End *pStart = new End(Position);

	pManager->AddStatement(pStart);
}