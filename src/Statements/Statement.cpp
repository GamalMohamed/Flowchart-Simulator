#include "Statement.h"

Statement::Statement()
{ 
	Text = "";
	comment = "";
	Selected = false;	
	Data = "";
	ID = 0;
	state = "";
	zoomed = false;

}


bool Statement::IsSelected() const
{
	return Selected;
}

void Statement::SetSelected(bool s)
{	Selected = s; }

void Statement::setID(int id)
{
	ID = id;
}

int Statement::getID()
{
	return ID;
}

void Statement::setComment(string c)
{
	comment = c;
}

string Statement::getText()
{
	return Text;
}

void Statement::setText(string t)
{
	Text = t;
}

void Statement::Comment(string s)
{
	(s != "") ? (comment = "//" + s) : (comment = " ");
}

void Statement::setState(string s)
{
	state = s;
}

string Statement::getState()
{
	return state;
}

Point Statement::getPosition()
{
	return position; 
}

int Statement::getRightEdge() const
{
	return RightEdge;
}

int Statement::getLeftEdge() const
{
	return LeftEdge;
}

int Statement::getWidth()
{
	return width;
}

int Statement::getHeight()
{
	return height;
}

double Statement::ParseAtom(char*& expr) const
{
	// Read the number from string
	char* end_ptr;
	double res = strtod(expr, &end_ptr);
	// Advance the pointer and return the result
	expr = end_ptr;
	return res;
}

// Parse multiplication and division
double Statement::ParseFactors(char*& expr) const
{
	double num1 = ParseAtom(expr);
	for (;;) {
		// Save the operation
		char op = *expr;
		if (op != '%' && op != '/' && op != '*')
			return num1;
		expr++;
		double num2 = ParseAtom(expr);
		// Perform the saved operation
		if (op == '%')
			num1 = fmod(num1, num2);
		else if (op == '/')
			num1 /= num2;
		else
			num1 *= num2;
	}
}

// Parse addition and subtraction
double Statement::ParseSummands(char*& expr) const
{
	double num1 = ParseFactors(expr);
	for (;;) {
		char op = *expr;
		if (op != '-' && op != '+')
			return num1;
		expr++;
		double num2 = ParseFactors(expr);
		if (op == '-')
			num1 -= num2;
		else
			num1 += num2;
	}
}

double Statement::EvaluateExpression(char* expr) const
{
	return ParseSummands(expr);
}

void Statement::ReplaceAll(string &s, const string &search, const string &replace) const
{
	for (size_t pos = 0;; pos += replace.length())
	{
		// Locate the substring to replace
		pos = s.find(search, pos);
		if (pos == string::npos)
			break;
		// Replace by erasing and inserting
		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

void Statement::setOldPos(Point pp)
{
	OldPos = pp;
}

Point Statement::getOldPos()
{
	return OldPos;
}