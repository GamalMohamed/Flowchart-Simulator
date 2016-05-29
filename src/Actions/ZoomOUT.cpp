#include "ZoomOUT.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

ZoomOUT::ZoomOUT(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void ZoomOUT::ReadActionParameters()
{
}

void ZoomOUT::Execute()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();


	pManager->setZoomMode(false);
	 
	for (int i = 0; i < pManager->StatCount; i++)
	{
		if (pManager->StatList[i]->getZoomed())
		{
			if (pManager->StatList[i]->getConn())
				pManager->StatList[i]->getConn()->setState("Not-Appended");

			pManager->StatList[i]->setWidth(pManager->StatList[i]->getOldWidth());
			pManager->StatList[i]->setHeight(pManager->StatList[i]->getOldHeight());
			pManager->StatList[i]->setPosition(pManager->StatList[i]->getOldPos());
			pManager->StatList[i]->setZoomed(false);
		}

	}
}