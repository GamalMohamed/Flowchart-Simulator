#include "Delete.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Delete::Delete(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Delete::ReadActionParameters()
{

}

void Delete::Execute()
{
	if (pManager->getZoomMode())
	{
		pManager->pOut->MsgBox("You can't delete in zoom mode !!", "Error", true);
		return;
	}


	// Delete each statements and preparing its connectors to be deleted
	for (int i = 0; i < pManager->SelectedCount; i++)
	{
		pManager->SelectedList[i]->SetSelected(false);
		pManager->SelectedList[i]->Delete(pManager->pOut);
		pManager->SelectedList[i]->setState("Deleted");
		pManager->StatList[i]->setZoomed(false);


		if (dynamic_cast<Start*>(pManager->SelectedList[i]))
			pManager->nS--;
		if (dynamic_cast<End*>(pManager->SelectedList[i]))
			pManager->nE--;

		pManager->pOut->ClearStatusBar();
	}

	for (int i = pManager->StatCount - 1; i >= 0; i--)
	{
		if (pManager->StatList[i]->getState() == "Deleted")
		{
			// Preparing the leaving connectors to be deleted
			if (dynamic_cast<Cond*>(pManager->StatList[i]))
			{
				if (dynamic_cast<Cond*>(pManager->StatList[i])->getConn1())
					pManager->AddSelectedConn(dynamic_cast<Cond*>(pManager->StatList[i])->getConn1());
				if (dynamic_cast<Cond*>(pManager->StatList[i])->getConn2())
					pManager->AddSelectedConn(dynamic_cast<Cond*>(pManager->StatList[i])->getConn2());
			}
			else
			{
				if (pManager->StatList[i]->getConn())
					pManager->AddSelectedConn(pManager->StatList[i]->getConn());
			}

			// Preparing the ingoing connectors to be deleted
			for (int j = 0; j < pManager->ConnCount; ++j)
			{
				if (pManager->ConnList[j]->getEndPoint().x == pManager->StatList[i]->getInlet().x
					&& pManager->ConnList[j]->getEndPoint().y == pManager->StatList[i]->getInlet().y)
					pManager->AddSelectedConn(pManager->ConnList[j]);
			}

			// Delete statement
			for (int j = i; j < pManager->StatCount; j++)
				pManager->StatList[j] = pManager->StatList[j + 1];
			pManager->StatCount--;
		}
	}

	for (int i = 0; i < pManager->StatCount; i++)
		pManager->StatList[i]->setID(i + 1);

	// Delete connectors
	for (int i = 0; i < pManager->ConnSelectedCount; i++)
	{
		pManager->ConnSelectedList[i]->getSrcStat()->setConn(NULL);
		pManager->ConnSelectedList[i]->SetSelected(false);
		pManager->pOut->ClearStatusBar();
	}

	for (int i = pManager->ConnCount - 1; i >= 0; i--)
	{
		for (int j = 0; j < pManager->ConnSelectedCount; j++)
		{
			if (pManager->ConnList[i] == pManager->ConnSelectedList[j])
			{
				for (int k = i; k < pManager->ConnCount; k++)
					pManager->ConnList[k] = pManager->ConnList[k + 1];
				pManager->ConnCount--;
				break;
			}
		}
	}

	pManager->EmptySelectedList();
	pManager->EmptyConnSelectedList();

	pManager->pOut->PrintMessage("Deleted Succesfully !");
}