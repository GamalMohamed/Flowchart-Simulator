#ifndef STATEMENT_H
#define STATEMENT_H

#include<fstream>
#include "..\defs.h"
#include "Connector.h"
//class Output;
#include "..\GUI\Output.h"
#include <map>
#include <set>
#include <vector>

//Base class for all Statements
class Statement
{
protected:
	int ID;			//Each Statement has an ID
	string Text;	//Statement text (e.g.  "X = 5" OR "if(salary > 3000)" and so on )
	bool Selected;	//true if the statement is selected on the folwchart
	string comment; //comment written on each statment 
	int height, width, OldH, OldW;
	string state;
	string Data;
	Point position, OldPos;
	bool zoomed;


	int RightEdge, LeftEdge;

	virtual void UpdateStatementText() = 0;	//is called when any part of the stat. is edited	

	/// Add more parameters if needed.

public:
	Statement();

	bool IsSelected() const;
	void SetSelected(bool s);
	void setID(int);
	int getID();
	void setComment(string);
	string getText();
	void setText(string);
	void setState(string);
    string getState();
	Point getPosition();
	int getWidth();
	int getHeight();
	virtual string CopyData() = 0;

	void setOldPos(Point);
	Point getOldPos();
	void setHeight(int H){ height = H; }
	void setWidth(int W){ width = W; }
	int getOldWidth(){ return OldW; }
	int getOldHeight(){ return OldH; }
	void setOldWidth(int w){ OldW = w; }
	void setOldHeight(int h){ OldH = h; }
	void setZoomed(bool z){ zoomed = z; }
	bool getZoomed(){ return zoomed; }
	virtual void zoomIn(Output*, int, int = 1) = 0;

	virtual bool setData(const string&) = 0;

	virtual void setPosition(Point) = 0;

	virtual void Draw(Output* pOut) = 0 ;	//Draw the statement

	void Comment(string);   //adds a comment to the statement

	int getRightEdge() const;               //get right edge of each item
	int getLeftEdge() const;               //get left edge of each item

	virtual Point getInlet() = 0;               //get inlet of each item
	virtual Point getOutlet() = 0;               //get outlet of each item
	virtual Connector* getConn() = 0;               //get the leaving connector
	virtual bool hasConn() = 0;               //check if the item has a connector
	virtual void setConn(Connector* conn) = 0;               //set pConn to the leaving connector

	
	
	///TODO:The following functions should be supported by the Statement class
	///		It should then be overridden by each derived Statement
	///		Decide the parameters that you should pass to each function and its return type	


	virtual string GenerateCode(Statement**, int, int, int, int&, int) = 0;	// Write the statement code to a file
	virtual void Simulate(Statement**, int, map<string, double>&, int, int&, bool&, Input*, Output*) const = 0;	// Execute the statement in the simulation mode


	virtual void PrintInfo(Output* pOut) const = 0;	//print all Statement info on the status bar

	virtual int Edit(string) = 0;		//Edit the Statement parameter

	virtual void Delete(Output*) = 0;    //deletes a statement from the flowchart

	virtual void Move(bool&, int&, int&, int&, int&, Connector**, int, Output*) = 0;		//Move the Statement on the flowchart

	virtual void Save(ofstream &OutFile) = 0;	//Save the Statement parameters to a file

	virtual void Load(ifstream &Infile, Output*) = 0;	//Load the Statement parameters from a file
	
	///TODO: Add more functions if needed


	double ParseAtom(char*&) const;

	double ParseFactors(char*&) const;

	double ParseSummands(char*&) const;

	double EvaluateExpression(char*) const;

	// Replace all substring occurrences in string
	void ReplaceAll(string&, const string&, const string&) const;
};

#endif