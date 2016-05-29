#include "EditStat.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

EditStat::EditStat(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void EditStat::ReadActionParameters()
{

}

void EditStat::Execute()
{
	int c = 0;

	if (pManager->SelectedCount > 0)
	{
		for (int i = 0; i < pManager->SelectedCount; i++)
		{

			if (dynamic_cast<Start*>(pManager->SelectedList[i]) || dynamic_cast<End*>(pManager->SelectedList[i]))
			{
				pManager->pOut->MsgBox("Start and End Statements can't be edited !! Click ok to continue editing process", "Error", true);

				for (int j = 0; j < pManager->StatCount; j++)
				{
					if (pManager->SelectedList[i] == pManager->StatList[j])
					{
						pManager->StatList[j]->SetSelected(false);
						pManager->StatList[j]->Draw(pManager->pOut);
						break;
					}
				}
			}

			else
			{
				while (1)
				{
					pManager->pOut->PrintMessage("Enter edited text for Selected item " + to_string(i + 1) + ": ");
					string s = pManager->pIn->GetString(pManager->pOut);
					int y = pManager->SelectedList[i]->Edit(s);
					if (y == 1)
					{
						for (int j = 0; j < pManager->StatCount; j++)
						{
							if (pManager->SelectedList[i] == pManager->StatList[j])
							{
								pManager->StatList[j]->SetSelected(false);
								pManager->StatList[j]->Edit(s);
								pManager->StatList[j]->Draw(pManager->pOut);
								break;
							}
						}
						break;
					}
					else if (y == -1)
					{
						pManager->pOut->MsgBox("Invalid text !! Click ok to Re-enter data", "Error", true);
					}
					c++;
				}
			}
		}

		pManager->EmptySelectedList();

		if (c > 0)
			pManager->pOut->PrintMessage("Edited Successfully !");
		else
			pManager->pOut->ClearStatusBar();
	}
	else
		pManager->pOut->MsgBox("You Should select the item(s) first then click on the action", "Info", false);
}