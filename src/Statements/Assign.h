#pragma once

#include "Statement.h"


//Simple Assignment statement class
//The simple assignment statement assigns a variable to a value
class Assign : public Statement
{
private:
	string LHS, RHS;
	
	Connector *pConn;	//Simple Assignment Stat. has one Connector to next statement

	Point Inlet;	//A point where connections enters this statement 
	Point Outlet;	//A point a connection leaves this statement


	virtual void UpdateStatementText();
	
public:
	Assign(Point = 0, string = "", double = 0);
	Assign(Statement*);
	
	void setLHS(const string&);
	void setRHS(const string&);
	bool setData(const string&);

	Point getInlet(){ return Inlet; }
	Point getOutlet(){ return Outlet; }
	Connector* getConn() { return pConn; }
	bool hasConn() { return pConn; }
	void setConn(Connector* conn) { pConn = conn; }


	virtual void Draw(Output*) ;


	virtual void PrintInfo(Output* pOut) const;

	virtual int Edit(string);

	virtual void Delete(Output*);

	virtual void setPosition(Point);

	virtual string CopyData();

	virtual void Move(bool&, int&, int&, int&, int&, Connector**, int, Output*);

	virtual void Save(ofstream &OutFile);

	virtual void Load(ifstream &Infile, Output*);

	virtual void Simulate(Statement**, int, map<string, double>&, int, int&, bool&, Input*, Output*) const;

	virtual string GenerateCode(Statement**, int, int, int, int&, int);

	virtual void zoomIn(Output*, int, int);

};

