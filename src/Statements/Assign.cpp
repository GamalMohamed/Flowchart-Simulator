#include "Assign.h"
#include <sstream>

using namespace std;

Assign::Assign(Point p, string LeftHS, double RightHS)
{
	width = UI.ASSGN_WDTH;
	height = UI.ASSGN_HI;

	Inlet = position = p;

	Outlet.x = Inlet.x;
	Outlet.y = position.y + height;

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - width / 2;

	LHS = LeftHS;
	RHS = RightHS;
	Data = "";
	pConn = NULL;	//No connectors yet !

	this->UpdateStatementText();
}

Assign::Assign(Statement* S)
{
	Text = S->getText();
	Selected = S->IsSelected();
	Data = S->CopyData();
	state = S->getState();
	position = S->getPosition();
	height = S->getHeight();
	width = S->getWidth();

	Inlet = position;

	Outlet.x = Inlet.x;
	Outlet.y = position.y + height;

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - width / 2;

	pConn = NULL;
}

void Assign::setLHS(const string &L)
{
	LHS = L;
	UpdateStatementText();
}

void Assign::setRHS(const string &R)
{
	RHS = R;
	UpdateStatementText();
}

bool Assign::setData(const string& d)
{
	string s1, s2; int I = 0;

	for (int i = 0;; i++) // Loop on LHS only
	{
		if ((d[i] >= 'A' && d[i] <= 'Z') || (d[i] >= 'a' && d[i] <= 'z') || d[i] == ' ')
			s1 += d[i];
		else if (d[i] == '=')
		{
			I = i;
			break;
		}
		else
			return false;
	}

	for (int i = I+1; i < d.length(); i++) // Loop on RHS only
	{
		if ((d[i] >= 'A' && d[i] <= 'Z') || (d[i] >= 'a' && d[i] <= 'z') || (d[i] >= '0' && d[i] <= '9') || (d[i] >= '*' && d[i] <= '/' && d[i] != ',') || d[i] == '%' || d[i] == '(' || d[i] == ')' || d[i] == ' ')
			s2 += d[i];
		else
			return false;
	}

	string tmp = s1;
	s1 = "";
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		s1 += tmp[i];
	}
	if (s1 == "")
		return false;

	tmp = s2;
	s2 = "";
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		s2 += tmp[i];
	}
	if (s2 == "")
		return false;

	enum ParseState
	{
		ExpectingAlphaOrDigitOrMinusOrDotOrOpen, // if prev is operator
		ExpectingAlphaOrOperatorOrClose, // if prev is alpha
		ExpectingDigitOrDotOrOperatorOrClose, // if prev is digit
		ExpectingDigitOrOperatorOrClose, // if prev is digit in fraction
		ExpectingAlphaOrDigitOrDot, // if prev is minus
		ExpectingDigit, // if prev is dot
		ExpectingAlphaOrDigitOrMinusOrDot, // if prev is opened brace
		ExpectingOperatorOrClose, // if prev is closed brace
	} state;

	char* str;

	// Check if RHS is a valid arithmetic expression
	state = ExpectingAlphaOrDigitOrMinusOrDotOrOpen;
	str = (char*)s2.c_str();
	do {
		if (*str != ' ')
			switch (state) {
			case ExpectingAlphaOrDigitOrMinusOrDotOrOpen: // if prev is operator
				if (isalpha(*str))
					state = ExpectingAlphaOrOperatorOrClose;
				else if (isdigit(*str))
					state = ExpectingDigitOrDotOrOperatorOrClose;
				else if (*str == '-')
					state = ExpectingAlphaOrDigitOrDot;
				else if (*str == '.')
					state = ExpectingDigit;
				else if (*str == '(')
					state = ExpectingAlphaOrDigitOrMinusOrDot;
				else
					return false;
				break;
			case ExpectingAlphaOrOperatorOrClose: // if prev is alpha
				if (isalpha(*str))
					state = ExpectingAlphaOrOperatorOrClose;
				else if (*str == '+' || *str == '-' || *str == '*' || *str == '/' || *str == '%')
					state = ExpectingAlphaOrDigitOrMinusOrDotOrOpen;
				else if (*str == ')')
					state = ExpectingOperatorOrClose;
				else
					return false;
				break;
			case ExpectingDigitOrDotOrOperatorOrClose: // if prev is digit
				if (isdigit(*str))
					state = ExpectingDigitOrDotOrOperatorOrClose;
				else if (*str == '.')
					state = ExpectingDigit;
				else if (*str == '+' || *str == '-' || *str == '*' || *str == '/' || *str == '%')
					state = ExpectingAlphaOrDigitOrMinusOrDotOrOpen;
				else if (*str == ')')
					state = ExpectingOperatorOrClose;
				else
					return false;
				break;
			case ExpectingDigitOrOperatorOrClose: // if prev is digit in fraction
				if (isdigit(*str))
					state = ExpectingDigitOrOperatorOrClose;
				else if (*str == '+' || *str == '-' || *str == '*' || *str == '/' || *str == '%')
					state = ExpectingAlphaOrDigitOrMinusOrDotOrOpen;
				else if (*str == ')')
					state = ExpectingOperatorOrClose;
				else
					return false;
				break;
			case ExpectingAlphaOrDigitOrDot: // if prev is minus
				if (isalpha(*str))
					state = ExpectingAlphaOrOperatorOrClose;
				else if (isdigit(*str))
					state = ExpectingDigitOrDotOrOperatorOrClose;
				else if (*str == '.')
					state = ExpectingDigit;
				else
					return false;
				break;
			case ExpectingDigit: // if prev is dot
				if (isdigit(*str))
					state = ExpectingDigitOrOperatorOrClose;
				else
					return false;
				break;
			case ExpectingAlphaOrDigitOrMinusOrDot: // if prev is opened brace
				if (isalpha(*str))
					state = ExpectingAlphaOrOperatorOrClose;
				else if (isdigit(*str))
					state = ExpectingDigitOrDotOrOperatorOrClose;
				else if (*str == '-')
					state = ExpectingAlphaOrDigitOrDot;
				else if (*str == '.')
					state = ExpectingDigit;
				else
					return false;
				break;
			case ExpectingOperatorOrClose: // if prev is closed brace
				if (*str == '+' || *str == '-' || *str == '*' || *str == '/' || *str == '%')
					state = ExpectingAlphaOrDigitOrMinusOrDotOrOpen;
				else if (*str == ')')
					state = ExpectingOperatorOrClose;
				else
					return false;
				break;
		}
	} while (*(++str));

	// Check if every opened brace has closed brace
	bool opened = false;
	for (int i = 0; i < s2.size(); i++)
	{
		if (s2[i] == '(' || s2[i] == ')')
		{
			if (s2[i] == '(' && opened)
				return false;
			if (s2[i] == ')' && !opened)
				return false;
			opened = s2[i] == '(';
		}
	}

	// Maximum variable length is 10
	char* cData = new char[s1.size() + 1];
	strcpy(cData, s1.c_str());
	char* var = strtok(cData, "+-*/%()");
	while (var) {
		if (strlen(var) > 10 && isalpha(var[0]))
			return false;
		var = strtok(NULL, "+-*/%()");
	}
	cData = new char[s2.size() + 1];
	strcpy(cData, s2.c_str());
	var = strtok(cData, "+-*/%()");
	while (var) {
		if (strlen(var) > 10 && isalpha(var[0]))
			return false;
		var = strtok(NULL, "+-*/%()");
	}
	delete[] cData;

	setLHS(s1);
	setRHS(s2);

	Data = s1 + " = " + s2;

	UpdateStatementText();
	return true;
}


void Assign::Draw(Output* pOut) 
{
	if (!zoomed)
		pOut->DrawAssign(position, width, height, Text, Selected);
	else
		pOut->DrawAssign(position, width, height, Text, Selected, true);

	// Update edges after updating dimensions
	RightEdge = position.x + width / 2;
	LeftEdge = position.x - width / 2;
}


//This function should be called when LHS or RHS changes
void Assign::UpdateStatementText()
{
	if (LHS == "")	//No left handside ==> statement have no valid text yet
		Text = "  =  ";
	else
		Text = Data;
}

void Assign::PrintInfo(Output* pOut) const
{
	pOut->PrintMessage("Assign: "+Data + "   " + comment + "   ID: " + to_string(ID));
}

int Assign::Edit(string s)
{
	bool y = setData(s);

	if (y)
		return 1;
	else
		return -1;
}

void Assign::Delete(Output* pOut)
{
	char w = 'A';
	pOut->ClearStat(position, w, width, height);
}

void Assign::setPosition(Point p)
{
	Inlet = position = p;

	Outlet.x = Inlet.x;
	Outlet.y = position.y + height;

	RightEdge = position.x + width / 2;
	LeftEdge = position.x - width / 2;
}

string Assign::CopyData()
{
	return Data;
}


void Assign::Move(bool& D, int& iX, int& iY, int& iXOld, int& iYOld, Connector** ConnList, int ConnCount, Output* pOut)
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

void  Assign::Save(ofstream &OutFile)
{
	OutFile << "ASSIGN" << "  " << ID << "  " << position.x - width / 2 << "  " << position.y << "  " << LHS << "  " << RHS << "  " << '"' << comment << '"' << endl;
}

void Assign::Load(ifstream &Infile, Output* pOut)
{
	int x;

	Infile >> ID >> x >> position.y >> LHS >> RHS;
	getline(Infile, comment);
	// Erasing first & last double quote
	comment.erase(comment.find('"'), 1);
	comment.erase(comment.rfind('"'), 1);

	string s = LHS + "=" + RHS;

	position.x = x + width / 2;
	setData(s);

	Inlet = position;
	RightEdge = position.x + width / 2;
	LeftEdge = position.x - width / 2;

	Outlet.x = Inlet.x;
	Outlet.y = position.y + height;

	Draw(pOut);
}

void Assign::Simulate(Statement** StatList, int StatCount, map<string, double> &variables, int curr_idx, int &next_idx, bool& zero, Input* pIn, Output* pOut) const
{
	string tmp, data;
	tmp = StatList[curr_idx]->CopyData();
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		data += tmp[i];
	}

	string Left = data.substr(0, data.find('='));
	string oldRHS = data.substr(data.find('=') + 1);

	string Right, v;
	for (int i = 0; i < oldRHS.size(); i++)
	{
		if (oldRHS[i] == '+' || oldRHS[i] == '-' || oldRHS[i] == '*' || oldRHS[i] == '/' || oldRHS[i] == '%' || oldRHS[i] == ')')
		{
			if (v.size())
			{
				Right += to_string(variables[v]);
				v.clear();
			}
			Right += oldRHS[i];
		}
		else if (isalpha(oldRHS[i]))
			v += oldRHS[i];
		else
			Right += oldRHS[i];
	}

	if (v.size())
	{
		Right += to_string(variables[v]);
		v.clear();
	}

	string var = Left;

	char* cRight = new char[Right.size() + 1];
	strcpy(cRight, Right.c_str());
	char* sub_expression = strtok(cRight, "()");
	while (sub_expression) {
		double val = EvaluateExpression(sub_expression);
		ostringstream ExpOld, ExpNew;
		ExpOld << "(" << sub_expression << ")";
		ExpNew << val;
		ReplaceAll(Right, ExpOld.str(), ExpNew.str());
		sub_expression = strtok(NULL, "()");
	}
	delete[] cRight;

	char *cRHS = new char[Right.size() + 1];
	strcpy(cRHS, Right.c_str());
	double val = EvaluateExpression(cRHS);
	delete[] cRHS;

	if (to_string(val).find('N') != string::npos || to_string(val).find('n') != string::npos)
		zero = true;
	variables[var] = val;

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

string Assign::GenerateCode(Statement** StatList, int StatCount, int end_idx, int curr_idx, int &next_idx, int level)
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

	string Left = data.substr(0, data.find('='));
	string Right = data.substr(data.find('=') + 1);

	if (comment.size())
		Code += tabs + comment + "\n";
	if (Left.size() && Right.size())
		Code += tabs + Left + " = " + Right + ";\n";

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

void Assign::zoomIn(Output* pOut, int n, int i)
{
	Point p;
	zoomed = true;

	if (n == 1)
	{
		p.x = UI.width / 2;
		p.y = UI.height / 2 - height / 2;
		setPosition(p);

		height *= 4;
		width *= 4;
	}

	else if (n == 2)
	{
		if (i == 1)
		{
			p.x = UI.width / 2;
			p.y = UI.height / 4 - UI.ASSGN_HI;
			setPosition(p);

			height *= 3;
			width *= 3;
		}


		if (i == 2)
		{
			p.x = UI.width / 2;
			p.y = 3 * UI.height / 4 - UI.ASSGN_HI;

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
			p.y = UI.height / 4 - UI.ASSGN_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 2)
		{
			p.x = UI.width / 4;

			p.y = 3 * UI.height / 4 - UI.ASSGN_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 3)
		{
			p.x = 3 * UI.width / 4;
			p.y = 3 * UI.height / 4 - UI.ASSGN_HI;
			setPosition(p);

			height *= 2;
			width *= 2;
		}
	}
	pOut->ClearDrawArea();
	Draw(pOut);
}