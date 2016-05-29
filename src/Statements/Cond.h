#pragma once

#include "Statement.h"

class Cond :public Statement
{
private:
	string LHS, RHS, cond;  // Data

	Connector *pConn1,*pConn2;	//Simple Assignment Stat. has one Connector to next statement

	Point Inlet;	//A point where connections enters this statement 
	Point Outlet1,Outlet2;	//A point a connection leaves this statement

	

	virtual void UpdateStatementText();

public:
	Cond(Point = 0, string = "", double = 0);

	Cond(Statement*);

	virtual void Draw(Output* pOut) ;

	void setLHS(const string&);
	void setRHS(const string&);
	string getCond();
	bool setCond(const string&);
	bool setData(const string&);


	Point getInlet(){ return Inlet; }
	Point getOutlet(){ return pConn1 ? Outlet2 : Outlet1; }
	Point getOutlet1(){ return Outlet1; }
	Point getOutlet2(){ return Outlet2; }
	Connector* getConn() { return pConn1; }
	Connector* getConn1() { return pConn1; }
	Connector* getConn2() { return pConn2; }
	bool hasConn() { return pConn1 && pConn2; }
	void setConn(Connector* conn);


	virtual void PrintInfo(Output* pOut) const;

	int Edit(string);

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