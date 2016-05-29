#include "Start.h"
using namespace std;


Start::Start(Point p)
{
	height = UI.START_HI;
	width = UI.START_WDTH;

	position = p;
	
	Outlet.x = position.x;
	Outlet.y = position.y + UI.START_HI;

	RightEdge = position.x + UI.START_WDTH / 2;
	LeftEdge = position.x - UI.START_WDTH / 2;

	pConn = NULL;	//No connectors yet !

	this->UpdateStatementText();
}

Start::Start(const Start& R)
{
	pConn = R.pConn;
	Outlet = R.Outlet;

	RightEdge = position.x + UI.START_WDTH / 2;
	LeftEdge = position.x - UI.START_WDTH / 2;
}

void Start::Draw(Output* pOut)
{
	if (!zoomed)
		pOut->DrawStart(position, width, height, Text, Selected);
	else
		pOut->DrawStart(position, width, height, Text, Selected, true);

	// Update edges after updating dimensions
	RightEdge = position.x + UI.START_WDTH / 2;
	LeftEdge = position.x - UI.START_WDTH / 2;
}


void Start::UpdateStatementText()
{	
	Text = "Start";
}

void Start::PrintInfo(Output* pOut) const
{
	pOut->PrintMessage(Text + "  " + comment + "   ID: " + to_string(ID));
}

int Start::Edit(string s)
{
	return 0;
}

void Start::Delete(Output* pOut)
{
	char w = 'S';
	pOut->ClearStat(position,w,width,height);
}

void Start::setPosition(Point p)
{
	position = p;

	Outlet.x = position.x;
	Outlet.y = position.y + UI.START_HI;

	RightEdge = position.x + UI.START_WDTH / 2;
	LeftEdge = position.x - UI.START_WDTH / 2;
}

void Start::Move(bool& D, int& iX, int& iY, int& iXOld, int& iYOld, Connector** ConnList, int ConnCount, Output* pOut)
{
	if (D == false)
	{
		if (((iX > position.x - width / 2) && (iX < (position.x + width / 2))) && ((iY > position.y) && (iY < (position.y + height))))
		{
			D = true;
			iXOld = iX; iYOld = iY;
		}
	}

	else if (D == true && (iX != iXOld || iY != iYOld))
	{
		// Move the leaving connector with the statement
		if (this->getConn())
			this->getConn()->Draw(pOut, true);

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
	}

}

void Start::Save(ofstream &OutFile)
{
	OutFile << "START" << "  " << ID << "  " << position.x << "  " << position.y + UI.START_HI << "  " << '"' << comment << '"' << endl;
}


void Start::Load(ifstream &Infile, Output* pOut)
{
	int pY = 0;
	Infile >> ID >> position.x >> pY;
	getline(Infile, comment);
	// Erasing first & last double quote
	comment.erase(comment.find('"'), 1);
	comment.erase(comment.rfind('"'), 1);

	position.y = pY - UI.START_HI;

	Outlet.x = position.x;
	Outlet.y = position.y + UI.START_HI;

	RightEdge = position.x + UI.START_WDTH / 2;
	LeftEdge = position.x - UI.START_WDTH / 2;

	Draw(pOut);

}

void Start::Simulate(Statement** StatList, int StatCount, map<string, double> &variables, int curr_idx, int &next_idx, bool& zero, Input* pIn, Output* pOut) const
{

}

string Start::GenerateCode(Statement** StatList, int StatCount, int end_idx, int curr_idx, int &next_idx, int level)
{
	string Code;
	Code += "#include <iostream>\nusing namespace std;\n";
	if (comment.size())
		Code += comment + "\n";
	Code += "int main()\n{\n";

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

void Start::zoomIn(Output* pOut, int n, int i)
{
	Point p;
	zoomed = true;

	if (n == 1)
	{
		p.x = UI.width / 2;
		p.y = UI.height / 2 - UI.START_HI / 2;
		setPosition(p);

		height *= 4;
		width *= 4;
	}

	else if (n == 2)
	{
		if (i == 1)
		{
			p.x = UI.width / 2;
			p.y = UI.height / 4 - UI.START_HI;
			setPosition(p);

			height *= 3;
			width *= 3;
		}


		if (i == 2)
		{
			p.x = UI.width / 2;
			p.y = 3 * UI.height / 4 - UI.START_HI;

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
			p.y = UI.height / 4 - UI.START_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 2)
		{
			p.x = UI.width / 4;

			p.y = 3 * UI.height / 4 - UI.START_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 3)
		{
			p.x = 3 * UI.width / 4;
			p.y = 3 * UI.height / 4 - UI.START_HI;
			setPosition(p);

			height *= 2;
			width *= 2;
		}




	}

	pOut->ClearDrawArea();
	Draw(pOut);
}