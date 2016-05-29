#include "End.h"
using namespace std;

End::End(Point p)
{
	width = UI.END_WDTH;
	height = UI.END_HI;

	Inlet = position = p;

	RightEdge = position.x + UI.END_WDTH / 2;
	LeftEdge = position.x - UI.END_WDTH / 2;

	pConn = NULL;	//No connectors yet !

	this->UpdateStatementText();
}

End::End(const End& R)
{
	pConn = R.pConn;
	Inlet = R.Inlet;

	RightEdge = position.x + UI.END_WDTH / 2;
	LeftEdge = position.x - UI.END_WDTH / 2;
}


void End::Draw(Output* pOut) 
{
	if (!zoomed)
		pOut->DrawEnd(position, width, height, Text, Selected);
	else
		pOut->DrawEnd(position, width, height, Text, Selected, true);

	// Update edges after updating dimensions
	RightEdge = position.x + UI.END_WDTH / 2;
	LeftEdge = position.x - UI.END_WDTH / 2;
}


void End::UpdateStatementText()
{
	Text = "End";
}


void End::PrintInfo(Output* pOut) const
{
	pOut->PrintMessage(Text  + "   " + comment + "   ID: " + to_string(ID));
}

int End::Edit(string s)
{
	return 0;
}

void End::Delete(Output* pOut)
{
	char w = 'S';
	pOut->ClearStat(position, w, width, height);

}


void End::setPosition(Point p)
{
	Inlet = position = p;

	RightEdge = position.x + UI.END_WDTH / 2;
	LeftEdge = position.x - UI.END_WDTH / 2;
}

void End::Move(bool& D, int& iX, int& iY, int& iXOld, int& iYOld, Connector** ConnList, int ConnCount, Output* pOut)
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
		Point oldInlet = this->getInlet();

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

void End::Save(ofstream &OutFile)
{
	OutFile << "END" << "  " << ID << "  " << position.x << "  " << position.y + UI.END_HI << "  " << '"' << comment << '"' << endl;
}


void End::Load(ifstream &Infile, Output* pOut)
{
	int pY = 0;
	Infile >> ID >> position.x >> pY;
	getline(Infile, comment);
	// Erasing first & last double quote
	comment.erase(comment.find('"'), 1);
	comment.erase(comment.rfind('"'), 1);

	position.y = pY - UI.END_HI;

	Inlet = position;

	RightEdge = position.x + UI.END_WDTH / 2;
	LeftEdge = position.x - UI.END_WDTH / 2;

	Draw(pOut);
}

void End::Simulate(Statement** StatList, int StatCount, map<string, double> &variables, int curr_idx, int &next_idx, bool& zero, Input* pIn, Output* pOut) const
{

}

string End::GenerateCode(Statement** StatList, int StatCount, int end_idx, int curr_idx, int &next_idx, int level)
{
	string tabs;
	for (int i = 0; i < level; ++i)
		tabs += "\t";

	string Code;
	if (comment.size())
		Code += tabs + comment + "\n";
	Code += tabs + "return 0;\n}";
	return Code;
}

void End::zoomIn(Output* pOut, int n, int i)
{
	Point p;
	zoomed = true;
	if (n == 1)
	{
		p.x = UI.width / 2;
		p.y = UI.height / 2 - UI.END_HI / 2;
		setPosition(p);

		height *= 4;
		width *= 4;

	}

	else if (n == 2)
	{
		if (i == 1)
		{
			p.x = UI.width / 2;
			p.y = UI.height / 4 - UI.END_HI;

			setPosition(p);

			height *= 3;
			width *= 3;
		}


		if (i == 2)
		{
			p.x = UI.width / 2;
			p.y = 3 * UI.height / 4 - UI.END_HI;

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
			p.y = UI.height / 4 - UI.END_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 2)
		{
			p.x = UI.width / 4;

			p.y = 3 * UI.height / 4 - UI.END_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 3)
		{
			p.x = 3 * UI.width / 4;
			p.y = 3 * UI.height / 4 - UI.END_HI;
			setPosition(p);

			height *= 2;
			width *= 2;
		}
	}

	pOut->ClearDrawArea();
	Draw(pOut);
}
