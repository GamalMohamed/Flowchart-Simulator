#include "Run.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <map>
#include <set>

using namespace std;

Run::Run(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Run::ReadActionParameters()
{
}

void Run::Execute()
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	Action* pVal = new Validate(pManager);
	string error;

	if (dynamic_cast<Validate*>(pVal)->isValid(error))
	{
		map<string, double> variables;
		map<string, bool> write;
		map<string, double>::iterator it;
		string watch;

		// Search for Start ID & End ID & Write Variables
		int start_index, end_index;
		for (int i = 0; i < pManager->getStatCount(); ++i)
		{
			if (dynamic_cast<Start*>(pManager->StatList[i]))
				start_index = i;
			if (dynamic_cast<End*>(pManager->StatList[i]))
				end_index = i;
			if (dynamic_cast<Write*>(pManager->StatList[i]))
			{
				string tmp, data;
				tmp = pManager->StatList[i]->CopyData();
				for (int i = 0; i < tmp.size(); i++)
				{
					if (tmp[i] == ' ')
						continue;
					data += tmp[i];
				}

				set<string> vars;
				set<string>::iterator it;
				char* cData = new char[data.size() + 1];
				strcpy(cData, data.c_str());
				char* var = strtok(cData, ",");
				while (var) {
					vars.insert(var);
					var = strtok(NULL, ",");
				}
				delete[] cData;

				for (it = vars.begin(); it != vars.end(); ++it)
					write[*it] = true;
			}
		}

		bool zero = false;
		int cnt = 0;
		bool infinite = false;

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
			
			// Simulate each statement and update variables values
			pManager->StatList[curr_idx]->Simulate(pManager->StatList, pManager->getStatCount(), variables, curr_idx, next_idx, zero, pIn, pOut);

			if (!zero)
			{
				watch.clear();
				for (it = variables.begin(); it != variables.end(); ++it)
				{
					//Uncomment the following line to show only write statements variables
					//if (write[it->first])
						watch += it->first + " = " + to_string(it->second) + "   ";
				}
			}
			
			if (curr_idx == end_index)
				break;

			if (zero)
			{
				pOut->MsgBox("Division by zero ! .. Run process will be terminated.", "Runtime error !", true);
				break;
			}

			if (cnt++ == 10000)
			{
				infinite = true;
				break;
			}
		}
		if (zero)
		{
			pOut->MsgBox("Abnormal termination due to runtime error (Division by zero) !", "Error", true);
			pOut->PrintMessage("Abnormal termination due to runtime error (Division by zero) !");
		}
		else if (infinite)
		{
			pOut->MsgBox("Abnormal termination due to a too long or an infinite loop !", "Error", true);
			pOut->PrintMessage("Abnormal termination due to a too long or an infinite loop !");
		}
		else
			pOut->PrintMessage("Last values: " + watch);
	}
	else
		pOut->MsgBox("Sorry. You can only run a valid flowchart. And this is NOT !", "Error", true);

	delete pVal;
}