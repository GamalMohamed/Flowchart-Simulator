#include "Save.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"
#include<vector>


using namespace std;

Save::Save(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Save::ReadActionParameters()
{
	
}

void Save::Execute()
{
	Output *pOut = pManager->GetOutput();

	ofstream Out;

	Out.open("Out.txt");

	Out << pManager->StatCount << endl;

	for (int i = 0; i <pManager->StatCount; i++)
		pManager->StatList[i]->Save(Out);

	Out << endl << endl;

	vector<int> C;

	Out << pManager->ConnCount << endl;

	for (int i = 0; i < pManager->ConnCount; i++)
	{
		/*1- Get src stat --> get it's ID
		2- Get dst stat --> get it's ID
		3- IF source is Conditional statement, set Outlet 1,2. Otherwise,set it to zero with any other statements.*/

		if (dynamic_cast<Cond*>(pManager->ConnList[i]->getSrcStat()))
		{
			if (std::find(C.begin(), C.end(), pManager->ConnList[i]->getSrcStat()->getID()) == C.end())
			{
				Out << pManager->ConnList[i]->getSrcStat()->getID() << "   " << pManager->ConnList[i]->getDstStat()->getID() << "   " << 1 << endl;
				C.push_back(pManager->ConnList[i]->getSrcStat()->getID());
			}
			else
				Out << pManager->ConnList[i]->getSrcStat()->getID() << "   " << pManager->ConnList[i]->getDstStat()->getID() << "   " << 2 << endl;
		}
		else
			Out << pManager->ConnList[i]->getSrcStat()->getID() << "   " << pManager->ConnList[i]->getDstStat()->getID() << "   " << 0 << endl;
	}

	Out.close();
	
	pOut->MsgBox("File saved successfully !", "Info", false);	
}