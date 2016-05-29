#include "Validate.h"

#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Validate::Validate(ApplicationManager *pAppManager) :Action(pAppManager)
{}

void Validate::ReadActionParameters()
{
}

void Validate::Execute()
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();

	string error;

	if (this->isValid(error))
	{
		string str = "Great news :D It's a valid flowchart !\n\nOne Start & One End.\nFully connected.\nNo syntax errors.\nNo division by zero.";
		pOut->MsgBox(str.c_str(), "Hooray !", false);
	}
	else
	{
		pOut->MsgBox(error.c_str(), "Error !", true);
	}
}

bool Validate::isValid(string& err)
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();

	err = "The flowchart has the following errors:\n";

	bool error = false;
	int errn = 1;

	bool start = pManager->nS == 1;
	bool end = pManager->nE == 1;

	if (!start)
	{
		err += "\n" + to_string(errn++) + ". No Start !";
		error = true;
	}

	if (!end)
	{
		err += "\n" + to_string(errn++) + ". No End !";
		error = true;
	}

	// Check for exactly one outgoing connector
	int no_outgoing = 0;
	for (int i = 0; i < pManager->getStatCount(); ++i)
		no_outgoing += !pManager->StatList[i]->hasConn();
	if (no_outgoing > 1)
	{
		err += "\n" + to_string(errn++) + ". " + to_string(no_outgoing - 1) + " statements with no outgoing connectors !";
		error = true;
	}

	// Check for at least one ingoing connector
	int no_ingoing = 0;
	for (int i = 0; i < pManager->getStatCount(); ++i)
	{
		bool has_ingoing = false;
		for (int j = 0; j < pManager->getConnCount(); ++j)
		{
			if (pManager->ConnList[j]->getEndPoint().x == pManager->StatList[i]->getInlet().x
				&& pManager->ConnList[j]->getEndPoint().y == pManager->StatList[i]->getInlet().y)
			{
				has_ingoing = true;
				break;
			}
		}
		no_ingoing += !has_ingoing;
	}
	if (no_ingoing > 1)
	{
		err += "\n" + to_string(errn++) + ". " + to_string(no_ingoing - 1) + " statements with no ingoing connectors !";
		error = true;
	}

	bool zero = false;
	for (int i = 0; i < pManager->getStatCount(); ++i)
	{
		string tmp, data;
		tmp = pManager->StatList[i]->CopyData();
		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp[i] == ' ')
				continue;
			data += tmp[i];
		}
		if (dynamic_cast<Assign*>(pManager->StatList[i]))
		{
			string RHS = data.substr(data.find('=') + 1);
			for (int i = 0; i < RHS.size(); i++)
			{
				if ((RHS[i] == '/' || RHS[i] == '%') && RHS[i + 1] == '0')
				{
					if (i + 1 == RHS.size() - 1)
						zero = true;
					else if (i + 2 < RHS.size() && !isdigit(RHS[i + 2]))
						zero = true;
				}
			}
		}
		else if (dynamic_cast<Cond*>(pManager->StatList[i]))
		{
			string cond = dynamic_cast<Cond*>(pManager->StatList[i])->getCond();
			string RHS = data.substr(data.find(cond) + cond.size());
			for (int i = 0; i < RHS.size(); i++)
			{
				if ((RHS[i] == '/' || RHS[i] == '%') && RHS[i + 1] == '0')
				{
					if (i + 1 == RHS.size() - 1)
						zero = true;
					else if (i + 2 < RHS.size() && !isdigit(RHS[i + 2]))
						zero = true;
				}
			}
		}
	}
	if (zero)
	{
		err += "\n" + to_string(errn++) + ". Runtime error (Division by zero) !";
		error = true;
	}

	return !error;
}