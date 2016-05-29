#include "ZoomIN.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

ZoomIN::ZoomIN(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void ZoomIN::ReadActionParameters()
{
}

void ZoomIN::Execute()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();

	
	if (pManager->ZM)
	{
		pOut->MsgBox("You can't zoom in more than that...It's nonsense !!", "Error", true);
		return;
	}
	
	pManager->setZoomMode(true);


	if (pManager->SelectedCount == 1)
	{
		pManager->SelectedList[0]->setOldPos(pManager->SelectedList[0]->getPosition());   //saves old position before zooming in 
		pManager->SelectedList[0]->setOldHeight(pManager->SelectedList[0]->getHeight()); //saves old height before zooming in 
		pManager->SelectedList[0]->setOldWidth(pManager->SelectedList[0]->getWidth());  //saves old width before zooming in 

		if (pManager->SelectedList[0]->getConn())
		{
			pManager->SelectedList[0]->getConn()->setState("Appended");
		}

		pManager->SelectedList[0]->zoomIn(pOut,1); 
	}

	else if (pManager->SelectedCount == 2)
	{
		pManager->SelectedList[0]->setOldPos(pManager->SelectedList[0]->getPosition()); 
		pManager->SelectedList[1]->setOldPos(pManager->SelectedList[1]->getPosition()); 

		pManager->SelectedList[0]->setOldHeight(pManager->SelectedList[0]->getHeight());
		pManager->SelectedList[0]->setOldWidth(pManager->SelectedList[0]->getWidth());

		pManager->SelectedList[1]->setOldHeight(pManager->SelectedList[1]->getHeight());
		pManager->SelectedList[1]->setOldWidth(pManager->SelectedList[1]->getWidth());

		if (pManager->SelectedList[0]->getConn())
		{
			pManager->SelectedList[0]->getConn()->setState("Appended");
		}

		if (pManager->SelectedList[1]->getConn())
		{
			pManager->SelectedList[1]->getConn()->setState("Appended");
		}

		pManager->SelectedList[0]->zoomIn(pOut, 2, 1);
		pManager->SelectedList[1]->zoomIn(pOut, 2, 2);
	}

	else if (pManager->SelectedCount == 3)
	{
		pManager->SelectedList[0]->setOldPos(pManager->SelectedList[0]->getPosition());
		pManager->SelectedList[1]->setOldPos(pManager->SelectedList[1]->getPosition());
		pManager->SelectedList[2]->setOldPos(pManager->SelectedList[2]->getPosition());


		pManager->SelectedList[0]->setOldHeight(pManager->SelectedList[0]->getHeight());
		pManager->SelectedList[0]->setOldWidth(pManager->SelectedList[0]->getWidth());

		pManager->SelectedList[1]->setOldHeight(pManager->SelectedList[1]->getHeight());
		pManager->SelectedList[1]->setOldWidth(pManager->SelectedList[1]->getWidth());

		pManager->SelectedList[2]->setOldHeight(pManager->SelectedList[2]->getHeight());
		pManager->SelectedList[2]->setOldWidth(pManager->SelectedList[2]->getWidth());

		if (pManager->SelectedList[0]->getConn())
		{
			pManager->SelectedList[0]->getConn()->setState("Appended");
		}

		if (pManager->SelectedList[1]->getConn())
		{
			pManager->SelectedList[1]->getConn()->setState("Appended");
		}

		if (pManager->SelectedList[2]->getConn())
		{
			pManager->SelectedList[2]->getConn()->setState("Appended");
		}


		pManager->SelectedList[0]->zoomIn(pOut, 3, 1);
		pManager->SelectedList[1]->zoomIn(pOut, 3, 2);
		pManager->SelectedList[2]->zoomIn(pOut, 3, 3);


	}

	else if (pManager->SelectedCount > 3)
	{
		pOut->MsgBox("You can't zoom in to more than 3 Statements...It's nonsense !!", "Error", true);
		pManager->ZM = false;
	}


}