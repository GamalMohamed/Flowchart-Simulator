#pragma once

#include "Statement.h"

//Read input statement class

class Read : public Statement
{
private:

	Connector *pConn;	//Read Stat. has one Connector to next statement

	Point Inlet;	//A point where connections enters this statement 
	Point Outlet;	//A point a connection leaves this statement

	

	virtual void UpdateStatementText();

public:
	Read(Point = 0);

	Read(Statement*);

	bool setData(const string& );

	virtual void Draw(Output* pOut) ;



	virtual void PrintInfo(Output* pOut) const;

	int Edit(string);

	virtual void Delete(Output*);

	virtual void setPosition(Point);

	virtual string CopyData();

	virtual void Move(bool&, int&, int&, int&, int&, Connector**, int, Output*);

	Point getInlet(){ return Inlet; }
	Point getOutlet(){ return Outlet; }
	Connector* getConn() { return pConn; }
	bool hasConn() { return pConn; }
	void setConn(Connector* conn) { pConn = conn; }

	virtual void Save(ofstream &OutFile);

	virtual void Load(ifstream &Infile, Output*);

	virtual void Simulate(Statement**, int, map<string, double>&, int, int&, bool&, Input*, Output*) const;

	virtual string GenerateCode(Statement**, int, int, int, int&, int);

	virtual void zoomIn(Output* pOut, int n, int);

};