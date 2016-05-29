#include "Debug.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <map>

using namespace std;

Debug::Debug(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Debug::ReadActionParameters()
{
}

void Debug::Execute()
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	Action* pVal = new Validate(pManager);
	string error;

	if (dynamic_cast<Validate*>(pVal)->isValid(error))
	{
		map<string, double> variables;
		map<string, double>::iterator it;
		string watch;

		// Search for Start & End IDs
		int start_index, end_index;
		bool found_start = false, found_end = false;
		for (int i = 0; i < pManager->getStatCount(); ++i)
		{
			if (dynamic_cast<Start*>(pManager->StatList[i]))
			{
				start_index = i;
				found_start = true;
			}
			if (dynamic_cast<End*>(pManager->StatList[i]))
			{
				end_index = i;
				found_end = true;
			}
			if (found_start && found_end)
				break;
		}

		bool zero = false;

		// Move after Start to End and track each variable
		int curr_idx, next_idx;
		int nextID = pManager->StatList[start_index]->getConn()->getDstStat()->getID();
		for (int i = 0; i < pManager->getStatCount(); ++i)
		{
			if (pManager->StatList[i]->getID() == nextID)
			{
				next_idx = i;
				break;
			}
		}
		curr_idx = next_idx;
		while (1)
		{
			curr_idx = next_idx;

			pManager->StatList[curr_idx]->SetSelected(true);
			pManager->StatList[curr_idx]->Draw(pOut);
			pOut->PrintMessage(watch + " Click anywhere to continue.");
			Point p;
			pIn->GetPointClicked(p);
			pManager->StatList[curr_idx]->SetSelected(false);
			pManager->StatList[curr_idx]->Draw(pOut);

			// Simulate each statement and update variables values
			pManager->StatList[curr_idx]->Simulate(pManager->StatList, pManager->getStatCount(), variables, curr_idx, next_idx, zero, pIn, pOut);

			if (!zero)
			{
				watch.clear();
				for (it = variables.begin(); it != variables.end(); ++it)
				{
					watch += it->first + " = " + to_string(it->second) + "   ";
				}
			}

			if (curr_idx == end_index)
				break;

			if (zero)
			{
				pOut->MsgBox("Division by zero ! .. Debug process will be terminated.", "Runtime Error", true);
				pOut->PrintMessage("Division by zero ! .. Debug process terminated.");
				break;
			}
		}
		if (zero)
		{
			pOut->MsgBox("Abnormal termination due to runtime error (Division by zero) !", "Error", true);
			pOut->PrintMessage("Abnormal termination due to runtime error (Division by zero) !");
		}
		else
			pOut->PrintMessage("Last values: " + watch);
	}
	else
		pOut->MsgBox("Sorry. You can only debug a valid flowchart. And this is NOT !", "Error", true);

	delete pVal;
}