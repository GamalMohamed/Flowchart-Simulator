#include "Read.h"
#include <sstream>

using namespace std;

Read::Read(Point p)
{
	width = UI.READ_WDTH;
	height = UI.READ_HI;

	Inlet = position = p;
	
	Outlet.x = position.x - width / 4;
	Outlet.y = position.y + height;

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - 3 * width / 4;

	Data = "";
	pConn = NULL;	//No connectors yet !

	this->UpdateStatementText();
}

Read::Read(Statement* S)
{
	Text = S->getText();
	Selected = S->IsSelected();
	Data = S->CopyData();
	state = S->getState();
	position = S->getPosition();
	height = S->getHeight();
	width = S->getWidth();

	Inlet = position;

	Outlet.x = position.x - width / 4;
	Outlet.y = position.y + height;

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - 3 * width / 4;

	pConn = NULL;
}

void Read::Draw(Output* pOut) 
{
	if (!zoomed)
		pOut->DrawRead(position, width, height, Text, Selected);
	else
		pOut->DrawRead(position, width, height, Text, Selected, true);

	// Update outlets & edges after updating dimensions
	Outlet.x = position.x - width / 4;
	Outlet.y = position.y + height;

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - 3 * width / 4;
}

void Read::UpdateStatementText()
{
	Text = "Read  " + Data;
}

bool Read::setData(const string &d)
{
	Data = "";
	for (int i = 0; i < d.length(); i++) 
	{
		if ((d[i] >= 'A' && d[i] <= 'Z') || (d[i] >= 'a' && d[i] <= 'z') || d[i] == ',' || d[i] == ' ')
			Data += d[i];
		else
			return false;
	}
	string tmp = Data;
	Data = "";
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		Data += tmp[i];
	}
	if (Data == "")
		return false;

	// Maximum variable length is 10
	char* cData = new char[Data.size() + 1];
	strcpy(cData, Data.c_str());
	char* var = strtok(cData, ",");
	while (var) {
		if (strlen(var) > 10)
			return false;
		var = strtok(NULL, ",");
	}
	delete[] cData;

	this->UpdateStatementText();
	return true;
}

void Read::PrintInfo(Output* pOut) const
{
	pOut->PrintMessage(Text  + "   " + comment + "   ID: " + to_string(ID));
}

int Read::Edit(string s)
{
	bool y = setData(s);

	if (y)
		return 1;
	else
		return -1;
}

void Read::Delete(Output* pOut)
{
	char w = 'R';
	pOut->ClearStat(position, w, width, height);

}

void Read::setPosition(Point p)
{
	Inlet = position = p;

	Outlet.x = position.x - width / 4;
	Outlet.y = position.y + height;

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - 3 * width / 4;
}

string Read::CopyData()
{
	return Data;
}

void Read::Move(bool& D, int& iX, int& iY, int& iXOld, int& iYOld, Connector** ConnList, int ConnCount, Output* pOut)
{
	if (D == false)
	{
		if (((iX > position.x - 3 * width / 4) && (iX < (position.x + width / 2))) && ((iY > position.y) && (iY < (position.y + height))))
		{
			D = true;
			iXOld = iX; iYOld = iY;
		}
	}
	else if (D == true && (iX != iXOld || iY != iYOld))
	{
		Point oldInlet = this->getInlet();

		// Move the leaving connector with the statement
		if (this->getConn())
			this->getConn()->Draw(pOut, true);

		// Move all the ingoing connectors with the statement
		for (int i = 0; i < ConnCount; ++i)
		{
			if (ConnList[i]->getEndPoint().x == oldInlet.x && ConnList[i]->getEndPoint().y == oldInlet.y)
				ConnList[i]->Draw(pOut, true);
		}

		Delete(pOut);
		Point new_pos(position.x + (iX - iXOld), position.y + (iY - iYOld));
		setPosition(new_pos);

		iXOld = iX;
		iYOld = iY;
		Draw(pOut);

		// Move the leaving connector with the statement
		if (this->getConn())
		{
			this->getConn()->setStartPoint(this->getOutlet());
			this->getConn()->Draw(pOut);
		}

		// Move all the ingoing connectors with the statement
		for (int i = 0; i < ConnCount; ++i)
		{
			if (ConnList[i]->getEndPoint().x == oldInlet.x && ConnList[i]->getEndPoint().y == oldInlet.y)
			{
				ConnList[i]->setEndPoint(this->getInlet());
				ConnList[i]->Draw(pOut);
			}
		}
	}
}

void Read::Save(ofstream &OutFile)
{
	OutFile << "READ" << "  " << ID << "  " << position.x - width / 2 << "  " << position.y << "  " << Data << "  " << '"' << comment << '"' << endl;
}

void Read::Load(ifstream &Infile, Output* pOut)
{
	int x;
	string s;
	Infile >> ID >> x >> position.y >> s;
	getline(Infile, comment);
	// Erasing first & last double quote
	comment.erase(comment.find('"'), 1);
	comment.erase(comment.rfind('"'), 1);

	position.x = x + width / 2;
	setData(s);

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - 3 * width / 4;

	Inlet = position;

	Outlet.x = position.x - width / 4;
	Outlet.y = position.y + height;

	Draw(pOut);

}

void Read::Simulate(Statement** StatList, int StatCount, map<string, double> &variables, int curr_idx, int &next_idx, bool& zero, Input* pIn, Output* pOut) const
{
	string tmp, data;
	tmp = StatList[curr_idx]->CopyData();
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		data += tmp[i];
	}

	vector<string> vars;
	char* cData = new char[data.size() + 1];
	strcpy(cData, data.c_str());
	char* var = strtok(cData, ",");
	while (var) {
		vars.push_back(var);
		var = strtok(NULL, ",");
	}
	delete[] cData;

	for (int i = 0; i < vars.size(); ++i)
	{
		string strVal;
		double val;
		while (1)
		{
			pOut->PrintMessage("Enter value of " + vars[i] + ": ");
			strVal = pIn->GetValue(pOut);
			bool validNum = true;

			if (strVal.size() == 1 && (strVal[0] == '-' || strVal[0] == '.'))
				validNum = false;

			if (validNum)
			{
				val = atof(strVal.c_str());
				break;
			}
			else
				pOut->MsgBox("Invalid number !! Click ok to Re-enter", "Error", true);
		}
		
		variables[vars[i]] = val;
	}

	int nextID = StatList[curr_idx]->getConn()->getDstStat()->getID();
	for (int i = 0; i < StatCount; ++i)
	{
		if (StatList[i]->getID() == nextID)
		{
			next_idx = i;
			break;
		}
	}
}

string Read::GenerateCode(Statement** StatList, int StatCount, int end_idx, int curr_idx, int &next_idx, int level)
{
	string tabs;
	for (int i = 0; i < level; ++i)
		tabs += "\t";

	string Code;
	string tmp, data;
	tmp = StatList[curr_idx]->CopyData();
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		data += tmp[i];
	}

	vector<string> vars;
	char* cData = new char[data.size() + 1];
	strcpy(cData, data.c_str());
	char* var = strtok(cData, ",");
	while (var) {
		vars.push_back(var);
		var = strtok(NULL, ",");
	}
	delete[] cData;

	if (comment.size())
		Code += tabs + comment + "\n";
	if (vars.size())
	{
		Code += tabs + "cin";
		for (int i = 0; i < vars.size(); ++i)
			Code += " >> " + vars[i];
		Code += ";\n";
	}

	int nextID = StatList[curr_idx]->getConn()->getDstStat()->getID();
	for (int i = 0; i < StatCount; ++i)
	{
		if (StatList[i]->getID() == nextID)
		{
			next_idx = i;
			break;
		}
	}
	return Code;
}

void Read::zoomIn(Output* pOut, int n, int i)
{
	Point p;
	zoomed = true;
	if (n == 1)
	{
		p.x = UI.width / 2 + width / 2;
		p.y = UI.height / 2 - 2 * height;
		setPosition(p);

		height *= 4;
		width *= 4;



	}

	else if (n == 2)
	{
		if (i == 1)
		{
			p.x = UI.width / 2;
			p.y = UI.height / 4 - UI.READ_HI;

			setPosition(p);

			height *= 3;
			width *= 3;
		}


		if (i == 2)
		{
			p.x = UI.width / 2;
			p.y = 3 * UI.height / 4 - UI.READ_HI;

			setPosition(p);

			height *= 3;
			width *= 3;
		}


	}

	else if (n == 3)
	{
		if (i == 1)
		{
			p.x = UI.width / 2;
			p.y = UI.height / 4 - UI.READ_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}


		if (i == 2)
		{
			p.x = UI.width / 4;
			p.y = 3 * UI.height / 4 - UI.READ_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 3)
		{
			p.x = 3 * UI.width / 4;
			p.y = 3 * UI.height / 4 - UI.READ_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}
	}
	pOut->ClearDrawArea();
	Draw(pOut);
}