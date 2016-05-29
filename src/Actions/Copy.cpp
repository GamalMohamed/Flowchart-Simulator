#include "Copy.h"
#include<string>

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Copy::Copy(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Copy::ReadActionParameters()
{
}

void Copy::Execute()
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();

	if (pManager->ZM)
	{
		pOut->MsgBox("You can't Copy Statements in Zoom mode !!", "Error", true);
		return;
	}

	Point P;
	int n = pManager->StatCount;

	for (int i = 0; i < pManager->SelectedCount; i++)
	{
		if (!(dynamic_cast<Start*>(pManager->SelectedList[i]) || dynamic_cast<End*>(pManager->SelectedList[i])))
		{
			pManager->SelectedList[i]->SetSelected(false);

			if (dynamic_cast<Read*>(pManager->SelectedList[i]))
			{
				Read *pT = new Read(pManager->SelectedList[i]);
				pManager->AddStatement(pT);
			}
			else if (dynamic_cast<Write*>(pManager->SelectedList[i]))
			{
				Write *pT = new Write(pManager->SelectedList[i]);
				pManager->AddStatement(pT);
			}
			else if (dynamic_cast<Assign*>(pManager->SelectedList[i]))
			{
				Assign *pT = new Assign(pManager->SelectedList[i]);
				pManager->AddStatement(pT);
			}
			else if (dynamic_cast<Cond*>(pManager->SelectedList[i]))
			{
				Cond *pT = new Cond(pManager->SelectedList[i]);
				pManager->AddStatement(pT);
			}

			pManager->StatList[pManager->StatCount - 1]->setState("Copy");
		}
		else
		{
			pOut->MsgBox("Start and End Statements can't be copied !! Click ok to continue copying process", "Error", true);
			
			pManager->SelectedList[i]->SetSelected(false);
			pManager->SelectedList[i]->Draw(pOut);
		}
	}

	pOut->PrintMessage("Copying finished !");

	pManager->EmptySelectedList();
}