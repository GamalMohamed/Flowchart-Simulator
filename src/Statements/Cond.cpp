#include "Cond.h"
#include <sstream>

using namespace std;

Cond::Cond(Point p, string LeftHS, double RightHS)
{
	width = UI.COND_WDTH;
	height = UI.COND_HI;
	
	Inlet = position = p;

	Outlet1.x = position.x + 0.5*width + 40;
	Outlet2.x = position.x - 0.5*width - 40;

	Outlet1.y = position.y + 0.5*height;
	Outlet2.y = position.y + 0.5*height;

	RightEdge = position.x + width / 2 + 20;
	LeftEdge = position.x - width / 2 - 20;

	LHS = LeftHS;
	RHS = RightHS;
	Data = "";
	pConn1 = pConn2 = NULL;	//No connectors yet !
}

Cond::Cond(Statement* S)
{
	Text = S->getText();
	Selected = S->IsSelected();
	Data = S->CopyData();
	state = S->getState();
	position = S->getPosition();
	height = S->getHeight();
	width = S->getWidth();

	Inlet = position;

	Outlet1.x = position.x + 0.5*width + 40;
	Outlet2.x = position.x - 0.5*width - 40;

	Outlet1.y = position.y + 0.5*height;
	Outlet2.y = position.y + 0.5*height;

	RightEdge = position.x + width / 2 + 20;
	LeftEdge = position.x - width / 2 - 20;

	pConn1 = NULL;
	pConn2 = NULL;
}


void Cond::setLHS(const string &L)
{
	LHS = L;
	UpdateStatementText();
}

string Cond::getCond()
{
	return cond;
}

bool Cond::setCond(const string &c)
{
	if (c == "==" || c == "<=" || c == ">=" || c == "!=" || c == "<" || c == ">")
	{
		cond = c;
		return true;
	}
	else
		return false;
}

void Cond::setRHS(const string &R)
{
	RHS = R;
	UpdateStatementText();
}

bool Cond::setData(const string& d)
{
	string s1, s2, s3; int I = 0;
	
	for (int i = 0; i < d.length(); i++) // Loop on LHS only
	{
		if ((d[i] >= 'A' && d[i] <= 'Z') || (d[i] >= 'a' && d[i] <= 'z') || (d[i] >= '0' && d[i] <= '9') || (d[i] >= '*' && d[i] <= '/' && d[i] != ',') || d[i] == '%' || d[i] == '(' || d[i] == ')' || d[i] == ' ')
			s1 += d[i];
		else if (d[i] == '=' || d[i] == '>' || d[i] == '<' || d[i] == '!')
		{
			I = i;
			break;
		}
		else
			return false;
	}

	for (int i = I; i < d.length(); i++) // Loop on cond & RHS
	{
		if (d[i] == '=' || d[i] == '>' || d[i] == '<' || d[i] == '!') 
			s2 += d[i];
		else if ((d[i] >= 'A' && d[i] <= 'Z') || (d[i] >= 'a' && d[i] <= 'z') || (d[i] >= '0' && d[i] <= '9') || (d[i] >= '*' && d[i] <= '/' && d[i] != ',') || d[i] == '%' || d[i] == '(' || d[i] == ')' || d[i] == ' ')
			s3 += d[i];
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

	tmp = s3;
	s3 = "";
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		s3 += tmp[i];
	}
	if (s3 == "")
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

	// Check if LHS is a valid arithmetic expression
	state = ExpectingAlphaOrDigitOrMinusOrDotOrOpen;
	str = (char*)s1.c_str();
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
				else
					return false;
				break;
		}
	} while (*(++str));

	// Check if RHS is a valid arithmetic expression
	state = ExpectingAlphaOrDigitOrMinusOrDotOrOpen;
	str = (char*)s3.c_str();
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
	for (int i = 0; i < s1.size(); i++)
	{
		if (s1[i] == '(' || s1[i] == ')')
		{
			if (s1[i] == '(' && opened)
				return false;
			if (s1[i] == ')' && !opened)
				return false;
			opened = s1[i] == '(';
		}
	}
	opened = false;
	for (int i = 0; i < s3.size(); i++)
	{
		if (s3[i] == '(' || s3[i] == ')')
		{
			if (s3[i] == '(' && opened)
				return false;
			if (s3[i] == ')' && !opened)
				return false;
			opened = s3[i] == '(';
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
	cData = new char[s3.size() + 1];
	strcpy(cData, s3.c_str());
	var = strtok(cData, "+-*/%()");
	while (var) {
		if (strlen(var) > 10 && isalpha(var[0]))
			return false;
		var = strtok(NULL, "+-*/%()");
	}
	delete[] cData;

	setLHS(s1);
	bool y = setCond(s2);
	setRHS(s3);

	if (y)
	{
		Data = s1 + ' ' + s2 + ' ' + s3;
		this->UpdateStatementText();
	}
	else
		return false;
}

void Cond::setConn(Connector* conn)
{
	if (conn)
	{
		if (pConn1)
			pConn2 = conn;
		else
			pConn1 = conn;
	}
	else
	{
		if (!pConn1)
			pConn2 = conn;
		else if (!pConn2)
			pConn1 = conn;
		else if (pConn1->IsSelected())
			pConn1 = conn;
		else
			pConn2 = conn;
	}
}

void Cond::Draw(Output* pOut) 
{
	if (!zoomed)
		pOut->DrawCondtional(position, width, height, Text, Selected);
	else
		pOut->DrawCondtional(position, width, height, Text, Selected, true);

	// Update outlets & edges after updating dimensions
	Outlet1.x = position.x + 0.5*width + 40;
	Outlet2.x = position.x - 0.5*width - 40;

	Outlet1.y = position.y + 0.5*height;
	Outlet2.y = position.y + 0.5*height;

	RightEdge = position.x + width / 2 + 20;
	LeftEdge = position.x - width / 2 - 20;
}

void Cond::UpdateStatementText()
{
	Text = Data;
}

void Cond::PrintInfo(Output* pOut) const
{
	pOut->PrintMessage( "Condition: "+Data + "   " + comment + "   ID: " + to_string(ID));
}

int Cond::Edit(string s)
{
	bool y = setData(s);

	if (y)
		return 1;
	else
		return -1;
}

void Cond::Delete(Output* pOut)
{
	char w = 'C';
	pOut->ClearStat(position, w, width, height);
}

void Cond::setPosition(Point p)
{
	Inlet = position = p;

	Outlet1.x = position.x + 0.5*width + 40;
	Outlet2.x = position.x - 0.5*width - 40;

	Outlet1.y = position.y + 0.5*height;
	Outlet2.y = position.y + 0.5*height;

	RightEdge = position.x + width / 2 + 20;
	LeftEdge = position.x - width / 2 - 20;
}


string Cond::CopyData()
{
	return Data;
}

void Cond::Move(bool& D, int& iX, int& iY, int& iXOld, int& iYOld, Connector** ConnList, int ConnCount, Output* pOut)
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

		// Move the leaving connectors with the statement
		if (this->getConn1())
			this->getConn1()->Draw(pOut, true);
		if (this->getConn2())
			this->getConn2()->Draw(pOut, true);

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

		// Move the leaving connectors with the statement
		if (this->getConn1())
		{
			this->getConn1()->setStartPoint(this->getOutlet1());
			this->getConn1()->Draw(pOut);
		}
		if (this->getConn2())
		{
			this->getConn2()->setStartPoint(this->getOutlet2());
			this->getConn2()->Draw(pOut);
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

void Cond::Save(ofstream &OutFile)
{
	string O;
	if (cond == "==")
		O = "E";
	else if (cond == "<=")
		O = "LSE";
	else if (cond == "!=")
		O = "NE";
	else if (cond == ">=")
		O = "GTRE";
	else if (cond == "<")
		O = "LS";
	else if (cond == ">")
		O = "GTR";

	OutFile << "COND" << "  " << ID << "  " << position.x << "  " << position.y << "  " << LHS << "  " << O << "  " << RHS << "  " << '"' << comment << '"' << endl;
}

void Cond::Load(ifstream &Infile, Output* pOut)
{
	string O;

	Infile >> ID >> position.x >> position.y >> LHS >> O >> RHS;
	getline(Infile, comment);
	// Erasing first & last double quote
	comment.erase(comment.find('"'), 1);
	comment.erase(comment.rfind('"'), 1);

	if (O == "E")
		cond = "==";
	else if (O == "LSE")
		cond = "<=";
	else if (O == "NE")
		cond = "!=";
	else if (O == "GTRE")
		cond = ">=";
	else if (O == "LS")
		cond = "<=";
	else if (O == "GTR")
		cond = ">";

	string s = LHS + cond + RHS;

	setData(s);

	RightEdge = position.x + width / 2 + 20;
	LeftEdge = position.x - width / 2 - 20;

	Inlet = position;

	Outlet1.x = position.x + 0.5*width + 40;
	Outlet2.x = position.x - 0.5*width - 40;

	Outlet1.y = position.y + 0.5*height;
	Outlet2.y = position.y + 0.5*height;

	Draw(pOut);
}

void Cond::Simulate(Statement** StatList, int StatCount, map<string, double> &variables, int curr_idx, int &next_idx, bool& zero, Input* pIn, Output* pOut) const
{
	string tmp, data;
	tmp = StatList[curr_idx]->CopyData();
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		data += tmp[i];
	}

	string cond = dynamic_cast<Cond*>(StatList[curr_idx])->getCond();
	string oldLHS = data.substr(0, data.find(cond));
	string oldRHS = data.substr(data.find(cond) + cond.size());

	string Left, Right, v;
	for (int i = 0; i < oldLHS.size(); i++)
	{
		if (oldLHS[i] == '+' || oldLHS[i] == '-' || oldLHS[i] == '*' || oldLHS[i] == '/' || oldLHS[i] == '%' || oldRHS[i] == ')')
		{
			if (v.size())
			{
				Left += to_string(variables[v]);
				v.clear();
			}
			Left += oldLHS[i];
		}
		else if (isalpha(oldLHS[i]))
			v += oldLHS[i];
		else
			Left += oldLHS[i];
	}
	if (v.size())
		Left += to_string(variables[v]);
	v.clear();

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
		Right += to_string(variables[v]);
	v.clear();


	char* cLeft = new char[Left.size() + 1];
	strcpy(cLeft, Left.c_str());
	char* sub_expression = strtok(cLeft, "()");
	while (sub_expression) {
		double val = EvaluateExpression(sub_expression);
		ostringstream ExpOld, ExpNew;
		ExpOld << "(" << sub_expression << ")";
		ExpNew << val;
		ReplaceAll(Left, ExpOld.str(), ExpNew.str());
		sub_expression = strtok(NULL, "()");
	}
	delete[] cLeft;

	char* cRight = new char[Right.size() + 1];
	strcpy(cRight, Right.c_str());
	sub_expression = strtok(cRight, "()");
	while (sub_expression) {
		double val = EvaluateExpression(sub_expression);
		ostringstream ExpOld, ExpNew;
		ExpOld << "(" << sub_expression << ")";
		ExpNew << val;
		ReplaceAll(Right, ExpOld.str(), ExpNew.str());
		sub_expression = strtok(NULL, "()");
	}
	delete[] cRight;

	char *cLHS = new char[Left.size() + 1];
	strcpy(cLHS, Left.c_str());
	double val1 = EvaluateExpression(cLHS);
	if (to_string(val1).find('N') != string::npos || to_string(val1).find('n') != string::npos)
		zero = true;
	delete[] cLHS;
	char *cRHS = new char[Right.size() + 1];
	strcpy(cRHS, Right.c_str());
	double val2 = EvaluateExpression(cRHS);
	if (to_string(val2).find('N') != string::npos || to_string(val2).find('n') != string::npos)
		zero = true;
	delete[] cRHS;

	bool yes = false;

	if (cond == "==")
		yes = val1 == val2;
	else if (cond == "!=")
		yes = val1 != val2;
	else if (cond == "<=")
		yes = val1 <= val2;
	else if (cond == ">=")
		yes = val1 >= val2;
	else if (cond == "<")
		yes = val1 < val2;
	else if (cond == ">")
		yes = val1 > val2;

	if (yes)
	{
		int nextID = dynamic_cast<Cond*>(StatList[curr_idx])->getConn1()->getDstStat()->getID();
		for (int i = 0; i < StatCount; ++i)
		{
			if (StatList[i]->getID() == nextID)
			{
				next_idx = i;
				break;
			}
		}
	}
	else
	{
		int nextID = dynamic_cast<Cond*>(StatList[curr_idx])->getConn2()->getDstStat()->getID();
		for (int i = 0; i < StatCount; ++i)
		{
			if (StatList[i]->getID() == nextID)
			{
				next_idx = i;
				break;
			}
		}
	}
}

string Cond::GenerateCode(Statement** StatList, int StatCount, int end_idx, int curr_idx, int &next_idx, int level)
{
	string tabs;
	for (int i = 0; i < level; ++i)
		tabs += "\t";

	int fake_next;
	string Code;
	string tmp, data;
	tmp = StatList[curr_idx]->CopyData();
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == ' ')
			continue;
		data += tmp[i];
	}
	
	string Left = data.substr(0, data.find(cond));
	string Right = data.substr(data.find(cond) + cond.size());

	if (comment.size())
		Code += tabs + comment + "\n";

	vector<int> YesIndexes, NoIndexes;
	bool YesIsLoop = false, NoIsLoop = false;

	// Get body of Yes-Branch as indexes
	int currYesIdx, nextYesIdx;
	int nextYesID = this->getConn1()->getDstStat()->getID();
	for (int i = 0; i < StatCount; ++i)
	{
		if (StatList[i]->getID() == nextYesID)
		{
			nextYesIdx = i;
			break;
		}
	}
	while (1)
	{
		currYesIdx = nextYesIdx;
		YesIndexes.push_back(currYesIdx);
		if (currYesIdx == curr_idx)
		{
			YesIsLoop = true;
			int nextID = this->getConn2()->getDstStat()->getID();
			for (int i = 0; i < StatCount; ++i)
			{
				if (StatList[i]->getID() == nextID)
				{
					next_idx = i;
					break;
				}
			}
			break;
		}
		if (currYesIdx == end_idx)
			break;
		StatList[currYesIdx]->GenerateCode(StatList, StatCount, end_idx, currYesIdx, nextYesIdx, level);
	}

	// Get body of No-Branch as indexes
	int currNoIdx, nextNoIdx;
	int nextNoID = this->getConn2()->getDstStat()->getID();
	for (int i = 0; i < StatCount; ++i)
	{
		if (StatList[i]->getID() == nextNoID)
		{
			nextNoIdx = i;
			break;
		}
	}
	while (1)
	{
		currNoIdx = nextNoIdx;
		NoIndexes.push_back(currNoIdx);
		if (currNoIdx == curr_idx)
		{
			NoIsLoop = true;
			int nextID = this->getConn1()->getDstStat()->getID();
			for (int i = 0; i < StatCount; ++i)
			{
				if (StatList[i]->getID() == nextID)
				{
					next_idx = i;
					break;
				}
			}
			break;
		}
		if (currNoIdx == end_idx)
			break;
		StatList[currNoIdx]->GenerateCode(StatList, StatCount, end_idx, currNoIdx, nextNoIdx, level);
	}

	string GeneratedYesCode, GeneratedNoCode;

	for (int i = 0; i < YesIndexes.size(); ++i)
	{
		if (YesIndexes[i] != curr_idx && find(NoIndexes.begin(), NoIndexes.end(), YesIndexes[i]) == NoIndexes.end())
			GeneratedYesCode += StatList[YesIndexes[i]]->GenerateCode(StatList, StatCount, end_idx, YesIndexes[i], fake_next, level + 1);
	}

	for (int i = 0; i < NoIndexes.size(); ++i)
	{
		if (NoIndexes[i] != curr_idx && find(YesIndexes.begin(), YesIndexes.end(), NoIndexes[i]) == YesIndexes.end())
			GeneratedNoCode += StatList[NoIndexes[i]]->GenerateCode(StatList, StatCount, end_idx, NoIndexes[i], fake_next, level + 1);
	}

	if (YesIsLoop)
	{
		Code += tabs + "while (" + Left + ' ' + cond + ' ' + Right + ")\n" + tabs + "{\n";
		Code += GeneratedYesCode;
		Code += tabs + "}\n";
	}
	else if (NoIsLoop)
	{
		Code += tabs + "while (!(" + Left + ' ' + cond + ' ' + Right + "))\n" + tabs + "{\n";
		Code += GeneratedNoCode;
		Code += tabs + "}\n";
	}
	else
	{
		Code += tabs + "if (" + Left + ' ' + cond + ' ' + Right + ")\n" + tabs + "{\n";
		Code += GeneratedYesCode;
		Code += tabs + "}\n" + tabs + "else\n" + tabs + "{\n";
		Code += GeneratedNoCode;
		Code += tabs + "}\n";

		bool nextFound = false;
		for (int i = 0; i < YesIndexes.size(); ++i)
		{
			if (find(NoIndexes.begin(), NoIndexes.end(), YesIndexes[i]) != NoIndexes.end())
			{
				next_idx = YesIndexes[i];
				nextFound = true;
				break;
			}
		}

		if (!nextFound)
		for (int i = 0; i < NoIndexes.size(); ++i)
		{
			if (find(YesIndexes.begin(), YesIndexes.end(), NoIndexes[i]) != YesIndexes.end())
			{
				next_idx = NoIndexes[i];
				break;
			}
		}
	}

	return Code;
}

void Cond::zoomIn(Output* pOut, int n, int i)
{
	Point p;
	zoomed = true;

	if (n == 1)
	{
		p.x = UI.width / 2;
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
			p.y = UI.height / 4 - UI.COND_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}


		if (i == 2)
		{
			p.x = UI.width / 2;
			p.y = 3 * UI.height / 4 - 2 * UI.COND_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}


	}

	else if (n == 3)
	{
		if (i == 1)
		{
			p.x = UI.width / 2;
			p.y = UI.height / 4 - UI.COND_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}


		if (i == 2)
		{
			p.x = UI.width / 4;
			p.y = 3 * UI.height / 4 - UI.COND_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}

		if (i == 3)
		{
			p.x = 3 * UI.width / 4;
			p.y = 3 * UI.height / 4 - UI.COND_HI;

			setPosition(p);

			height *= 2;
			width *= 2;
		}
	}

	pOut->ClearDrawArea();
	Draw(pOut);
}