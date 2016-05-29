#include "Select.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;


Select::Select(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Select::ReadActionParameters()
{

}

void Select::Execute()
{
	Statement*pS = pManager->GetStatement(pManager->p);
	Connector*pC = pManager->GetConnector(pManager->p);

	if (pS || pC)
	{
		if (pS)
		{
			for (int i = 0; i < pManager->StatCount; i++)
			{
				if (pS == pManager->StatList[i] && !(pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(true);
					pManager->StatList[i]->PrintInfo(pManager->pOut);
					pManager->AddSelected(pManager->StatList[i]);
					break;
				}
				if (pS == pManager->StatList[i] && (pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(false);
					pManager->pOut->ClearStatusBar();
					pManager->RemoveSelected(pManager->StatList[i]);
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < pManager->ConnCount; i++)
			{
				if (pC == pManager->ConnList[i] && !pManager->ConnList[i]->IsSelected())
				{
					pManager->ConnList[i]->SetSelected(true);
					pManager->AddSelectedConn(pManager->ConnList[i]);
					break;
				}
				if (pC == pManager->ConnList[i] && pManager->ConnList[i]->IsSelected())
				{
					pManager->ConnList[i]->SetSelected(false);
					pManager->RemoveSelectedConn(pManager->ConnList[i]);
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < pManager->StatCount; i++)
		{
			pManager->StatList[i]->SetSelected(false);
		}
		pManager->EmptySelectedList();

		for (int i = 0; i < pManager->ConnCount; i++)
		{
			pManager->ConnList[i]->SetSelected(false);
		}
		pManager->EmptyConnSelectedList();

		pManager->pOut->ClearStatusBar();
	}

}