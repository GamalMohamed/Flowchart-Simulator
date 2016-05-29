#include "AddStart.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

//#include <sstream>

using namespace std;

AddStart::AddStart(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void AddStart::ReadActionParameters()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();

	pOut->PrintMessage("Start Statement: Click to add the statement");

	while (1)
	{
		pIn->GetPointClicked(Position);

		if (Position.y > UI.TlBrWdth && Position.y < UI.height - UI.StBrWdth)
		{
			//Calculating each corner of the statement block then check if it's inside another statement
			Point C1(Position.x - UI.START_WDTH / 2, Position.y);
			Point C2(Position.x + UI.START_WDTH / 2, Position.y);
			Point C3(Position.x - UI.START_WDTH / 2, Position.y + UI.START_HI);
			Point C4(Position.x + UI.START_WDTH / 2, Position.y + UI.START_HI);

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

void AddStart::Execute()
{
	this->ReadActionParameters();

	Start *pStart = new Start(Position);

	pManager->AddStatement(pStart);
}
