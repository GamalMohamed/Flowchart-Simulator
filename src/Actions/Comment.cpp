#include "Comment.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Comment::Comment(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Comment::ReadActionParameters()
{

}

void Comment::Execute()
{
	if (pManager->SelectedCount > 0)
	{
		for (int i = 0; i < pManager->SelectedCount; i++)
		{
			pManager->pOut->PrintMessage("Enter comment for Selected item " + to_string(i + 1) + ": ");
			string s = pManager->pIn->GetString(pManager->pOut);
			for (int j = 0; j < pManager->StatCount; j++)
			{
				if (pManager->SelectedList[i] == pManager->StatList[j])
				{
					pManager->StatList[j]->SetSelected(false);
					pManager->StatList[j]->Comment(s);
					pManager->StatList[j]->Draw(pManager->pOut);
					break;
				}
			}
		}

		pManager->EmptySelectedList();

		pManager->pOut->ClearStatusBar();
	}
	else
		pManager->pOut->MsgBox("You Should select the item(s) first then click on the action", "Info", false);
}