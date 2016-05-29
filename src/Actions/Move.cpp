#include "Move.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Move::Move(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Move::ReadActionParameters()
{
}

void Move::Execute()
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();

	if (pManager->ZM)
	{
		pOut->MsgBox("You can't move Statements in zoom mode !!", "Error", true);
		return;
	}

	for (int i = 0; i < pManager->SelectedCount; i++)
	{
		pManager->SelectedList[i]->Draw(pOut);

		bool Dragging = false;
		int iX, iY, iXOld, iYOld;
		iX = iY = iXOld = iYOld = 0;

		pOut->PrintMessage("Drag item " + to_string(i + 1));

		while (1)
		{
			if (Dragging == false)
			{
				if (pIn->Getbuttonstate(LEFT_BUTTON, iX, iY) == BUTTON_DOWN)      //within range-->same position
					pManager->SelectedList[i]->Move(Dragging, iX, iY, iXOld, iYOld, pManager->ConnList, pManager->ConnCount, pOut);
			}
			else
			{
				if (pIn->Getbuttonstate(LEFT_BUTTON, iX, iY) == BUTTON_UP)
				{
					Dragging = false;
					pManager->SelectedList[i]->SetSelected(false);
					pManager->SelectedList[i]->Draw(pOut);
					break;
				}
				else
				{
					pManager->SelectedList[i]->Move(Dragging, iX, iY, iXOld, iYOld, pManager->ConnList, pManager->ConnCount, pOut);
				}
			}
		}
	}

	pOut->PrintMessage("Item(s) moved successfully !");

	pManager->EmptySelectedList();
}