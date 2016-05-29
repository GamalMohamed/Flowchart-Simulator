#pragma once
#include "Statement.h"


class Start :public Statement
{
private:
	
	Connector *pConn;	//Start Stat. has one Connector to next statement
	Point Outlet;	  //A point a connection leaves this statement

	virtual void UpdateStatementText();

public:
	Start(Point = 0);
	Start(const Start&);


	virtual void Draw(Output* pOut);

	virtual void PrintInfo(Output* pOut) const;

	int Edit(string);

	virtual void Delete(Output*);

	virtual void setPosition(Point);

	virtual string CopyData(){ return ""; }

	virtual bool setData(const string&){ return false; }

	virtual void Move(bool&, int&, int&, int&, int&, Connector**, int, Output*);

	Point getInlet(){ return NULL; }
	Point getOutlet(){ return Outlet; }
	Connector* getConn() { return pConn; }
	bool hasConn() { return pConn; }
	void setConn(Connector* conn) { pConn = conn; }

	virtual void Save(ofstream &OutFile);

	virtual void Load(ifstream &Infile, Output*);

	virtual void Simulate(Statement**, int, map<string, double>&, int, int&, bool&, Input*, Output*) const;

	virtual string GenerateCode(Statement**, int, int, int, int&, int);

	virtual void zoomIn(Output*, int, int);

};
