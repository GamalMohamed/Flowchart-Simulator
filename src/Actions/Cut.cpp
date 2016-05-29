#include "Cut.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Cut::Cut(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Cut::ReadActionParameters()
{
}

void Cut::Execute()
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();

	if (pManager->ZM)
	{
		pOut->MsgBox("You can't Cut Statements in zoom mode !!", "Error", true);
		return;
	}
	
	for (int i = 0; i < pManager->SelectedCount; i++)
	{
		pManager->SelectedList[i]->SetSelected(false);
		pManager->SelectedList[i]->setState("Cut");
		pManager->SelectedList[i]->Delete(pOut);
		pManager->pOut->ClearStatusBar();
	}

	for (int i = pManager->StatCount - 1; i >= 0; i--)
	{
		if (pManager->StatList[i]->getState() == "Cut")
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
		}
	}

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

	pOut->PrintMessage("Cutting finished !");
}