#include "AddConnector.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

AddConnector::AddConnector(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void AddConnector::ReadActionParameters()
{
	
}

void AddConnector::Execute()
{
	Output *pOut = pManager->GetOutput();

	if (pManager->getZoomMode())
	{
		pOut->MsgBox("You can't Add Connectors in zoom mode !!", "Error", true);
		return;
	}

	int SelectedCount = pManager->getSelectedCount();
	Statement** SelectedList = pManager->GetAllSelected();

	if (SelectedCount == 2)
	{
		if (SelectedList[0]->hasConn())
			pOut->MsgBox("Invalid! The selected source already has a connector to another one.", "Error", true);
		else if (dynamic_cast<End*>(SelectedList[0]))
			pOut->MsgBox("Invalid! The source can't be the End statement.", "Error", true);
		else if (dynamic_cast<Start*>(SelectedList[1]))
			pOut->MsgBox("Invalid! The destination can't be the Start statement.", "Error", true);
		else 
		{
			Connector *pConnector = new Connector(SelectedList[0], SelectedList[1]);
			pConnector->setStartPoint(SelectedList[0]->getOutlet());
			pConnector->setEndPoint(SelectedList[1]->getInlet());
			SelectedList[0]->setConn(pConnector);
			pManager->AddConn(pConnector);
			SelectedList[1]->SetSelected(false);
			pManager->RemoveSelected(SelectedList[1]);
			SelectedList[0]->SetSelected(false);
			pManager->RemoveSelected(SelectedList[0]);
			pOut->ClearStatusBar();
		}
	}
	else
		pOut->MsgBox("You should select exactly 2 items then click on this action", "Info", false);
}
