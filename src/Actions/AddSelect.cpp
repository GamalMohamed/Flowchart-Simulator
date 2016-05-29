#include "AddSelect.h"


#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;


AddSelect::AddSelect(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void AddSelect::ReadActionParameters()
{
	Input *pIn = pManager->GetInput();
	Output *pOut = pManager->GetOutput();

	pIn->GetPointClicked(Position);

	pOut->ClearStatusBar();
}

void AddSelect::Execute()
{
	ReadActionParameters();
	Output *pOut = pManager->GetOutput();


	for (int i = 0; i < pManager->StatCount; i++)
	{
		Point p = pManager->StatList[i]->getPosition();

		if (dynamic_cast<Start*> (pManager->StatList[i]))
		{
			if (((Position.x >= p.x - 0.5*UI.START_WDTH) && (Position.x <= p.x + 0.5*UI.START_WDTH)) && ((Position.y >= p.y) && (Position.y <= p.y + UI.START_HI)))
			{
				if (!(pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(true);
					pManager->StatList[i]->Draw(pOut);
					pManager->AddSelected(pManager->StatList[i]);
					pManager->StatList[i]->PrintInfo(pOut);
				}
				else if ((pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(false);
					pManager->StatList[i]->Draw(pOut);
					pManager->RemoveSelected(pManager->StatList[i]);
					pOut->ClearStatusBar();
				}
			}
		}

		else if (dynamic_cast<End*> (pManager->StatList[i]))
		{
			if (((Position.x >= p.x - 0.5*UI.END_WDTH) && (Position.x <= p.x + 0.5*UI.END_WDTH)) && ((Position.y >= p.y) && (Position.y <= p.y + UI.END_HI)))
			{
				if (!(pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(true);
					pManager->StatList[i]->Draw(pOut);

					pManager->AddSelected(pManager->StatList[i]);
					pManager->StatList[i]->PrintInfo(pOut);
				}
				else if ((pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(false);
					pManager->StatList[i]->Draw(pOut);
					pManager->RemoveSelected(pManager->StatList[i]);
					pOut->ClearStatusBar();
				}
			}
		}

		else if (dynamic_cast<Assign*> (pManager->StatList[i]))
		{
			if (((Position.x >= p.x - 0.5*UI.ASSGN_WDTH) && (Position.x <= p.x + 0.5*UI.ASSGN_WDTH)) && ((Position.y >= p.y) && (Position.y <= p.y + UI.ASSGN_HI)))
			{
				if (!(pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(true);
					pManager->StatList[i]->Draw(pOut);
					pManager->AddSelected(pManager->StatList[i]);
					pManager->StatList[i]->PrintInfo(pOut);
				}
				else if ((pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(false);
					pManager->StatList[i]->Draw(pOut);
					pManager->RemoveSelected(pManager->StatList[i]);
					pOut->ClearStatusBar();
				}
			}
		}

		else if (dynamic_cast<Read*> (pManager->StatList[i]))
		{
			if (((Position.x >= p.x - 0.75*UI.READ_WDTH) && (Position.x <= p.x + 0.5*UI.READ_WDTH)) && ((Position.y >= p.y) && (Position.y <= p.y + UI.READ_HI)))
			{
				if (!(pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(true);
					pManager->StatList[i]->Draw(pOut);
					pManager->AddSelected(pManager->StatList[i]);
					pManager->StatList[i]->PrintInfo(pOut);
				}
				else if ((pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(false);
					pManager->StatList[i]->Draw(pOut);
					pManager->RemoveSelected(pManager->StatList[i]);
					pOut->ClearStatusBar();
				}
			}
		}

		else if (dynamic_cast<Write*> (pManager->StatList[i]))
		{
			if (((Position.x >= p.x - 0.75*UI.WRITE_WDTH) && (Position.x <= p.x + 0.5*UI.WRITE_WDTH)) && ((Position.y >= p.y) && (Position.y <= p.y + UI.WRITE_HI)))
			{
				if (!(pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(true);
					pManager->StatList[i]->Draw(pOut);
					pManager->AddSelected(pManager->StatList[i]);
					pManager->StatList[i]->PrintInfo(pOut);
				}
				else if ((pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(false);
					pManager->StatList[i]->Draw(pOut);
					pManager->RemoveSelected(pManager->StatList[i]);
					pOut->ClearStatusBar();
				}
			}
		}
		else if (dynamic_cast<Cond*> (pManager->StatList[i]))
		{
			if (Position.y >= p.y  && Position.x <= p.x + UI.COND_WDTH / 2 && Position.x >= p.x - UI.COND_WDTH / 2 && Position.y <= p.y + UI.COND_HI)
			{
				if (!(pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(true);
					pManager->StatList[i]->Draw(pOut);
					pManager->AddSelected(pManager->StatList[i]);
					pManager->StatList[i]->PrintInfo(pOut);
				}
				else if ((pManager->StatList[i]->IsSelected()))
				{
					pManager->StatList[i]->SetSelected(false);
					pManager->StatList[i]->Draw(pOut);
					pManager->RemoveSelected(pManager->StatList[i]);
					pOut->ClearStatusBar();
				}
			}
		}

	}


	//else if (dynamic_cast<Connector*> (pManager->StatList[i])); 
}
	




/*if ((Position.x >= p.x - UI.READ_WDTH / 2) && (Position.x <= p.x + UI.READ_WDTH / 2) && (Position.x <= p.x + UI.READ_WDTH / 4) && (Position.x >= p.x - 3 * UI.READ_WDTH / 4) 
&& (Position.y >= p.y) && (Position.y <= p.y + UI.READ_HI)) */
//else
//{
//	for (int i = 0; i < pManager->SelectedCount; i++)
//	{
//		pManager->SelectedList[i]->SetSelected(false);
//		pManager->SelectedList[i]->Draw(pOut);
//		pManager->RemoveSelected(pManager->SelectedList[i]);
//		pOut->ClearStatusBar();
//	}
//}
