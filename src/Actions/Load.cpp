#include "Load.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"
#include<vector>

using namespace std;


Load::Load(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Load::ReadActionParameters()
{

}

void Load::Execute()
{
	Output *pOut = pManager->GetOutput();

	if (pManager->StatCount>0)
	{
		pOut->MsgBox("You Should delete any existing statements before loading a new flowchart !!", "Error", true);
		return;
	}

	ifstream In;

	In.open("Out.txt");

	if (!In.good())
	{
		pOut->MsgBox("File couldn't be Loaded !!", "Error", true);
		return;
	}

	int N1;
	In >> N1;

	string ST;
     
	for (int i = 0; i <N1; i++)
	{
		In >> ST;
		if (ST=="START")
		{
			Start* pT = new Start;
			pT->Load(In, pOut);
			pManager->AddStatement(pT,true);

		}

		else if (ST=="END")
		{
			End* pT = new End;
			pT->Load(In,pOut);
			pManager->AddStatement(pT, true);

		}

		else if (ST == "ASSIGN")
		{
			Assign* pT = new Assign;
			pT->Load(In, pOut);
			pManager->AddStatement(pT, true);

		}

		else if (ST == "COND")
		{
			Cond* pT = new Cond;
			pT->Load(In, pOut);
			pManager->AddStatement(pT, true);
		}

		else if (ST == "WRITE")
		{
			Write* pT = new Write;
			pT->Load(In, pOut);
			pManager->AddStatement(pT, true);
		}

		else if (ST == "READ")
		{
			Read* pT = new Read;
			pT->Load(In, pOut);
			pManager->AddStatement(pT, true);
		}
	}

	
	int N2;
	In >> N2;

	for (int i = 0; i < N2; i++)
	{
		int s, d,t;
		In >> s >> d>>t;
		Statement *S, *D; S = D = NULL;
		Point p1, p2;

		for (int j = 0; j < pManager->StatCount; j++)
		{
			if (s == pManager->StatList[j]->getID())
			{
				S = pManager->StatList[j];
				p1 = pManager->StatList[j]->getOutlet();
			}

			if (d == pManager->StatList[j]->getID())
			{
				D = pManager->StatList[j];
				p2 = pManager->StatList[j]->getInlet();
			}

			if (S && D)
				break;
		}

		/*Connector* pT = new Connector(S,D);
		pT->setStartPoint(p1);
		pT->setEndPoint(p2);
		pManager->AddConn(pT);
		pT->Draw(pManager->pOut);*/

		pManager->AddSelected(S);
		pManager->AddSelected(D);

		Action* pAct = NULL;
		pAct = new AddConnector(pManager);
		pAct->Execute();
	}


	In.close();

	pOut->MsgBox("File Loaded successfully !", "Info", false);
}