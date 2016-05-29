#include "Paste.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Paste::Paste(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Paste::ReadActionParameters()
{
}

void Paste::Execute()
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	Point P;
	int c = 0;


	for (int i = 0; i < pManager->StatCount; i++)
	{
		if (pManager->StatList[i]->getState() == "Cut" || pManager->StatList[i]->getState() == "Copy")
		{
			pOut->PrintMessage("Click the destination place for item" + to_string(i + 1) + " : ");

			while (1)
			{
				pIn->GetPointClicked(P);

				if (P.y > UI.TlBrWdth && P.y < UI.height - UI.StBrWdth)
				{
					//Calculating each corner of the statement block then check if it's inside another statement
					Point C1(P.x - pManager->StatList[i]->getWidth() / 2, P.y);
					Point C2(P.x + pManager->StatList[i]->getWidth() / 2, P.y);
					Point C3(P.x - pManager->StatList[i]->getWidth() / 2, P.y + pManager->StatList[i]->getHeight());
					Point C4(P.x + pManager->StatList[i]->getWidth() / 2, P.y + pManager->StatList[i]->getHeight());

					if (pManager->GetStatement(C1) == NULL && pManager->GetStatement(C2) == NULL && pManager->GetStatement(C3) == NULL && pManager->GetStatement(C4) == NULL)
					{
						pOut->ClearStatusBar();
						break;
					}
					else
						pOut->PrintMessage("Can't paste statement near another one !! Click in another valid position");
				}
				else
					pOut->PrintMessage("Invalid area !! Click in another valid position");
			}

			pManager->StatList[i]->setPosition(P);
			pManager->StatList[i]->SetSelected(false);
			pManager->StatList[i]->Draw(pOut);
			pManager->StatList[i]->setState("");
			c++;
		}
	}
	
	if (c>0)
		pOut->PrintMessage("Pasted all items !");
	else
		pOut->PrintMessage("Clipboard is Empty !!");
}