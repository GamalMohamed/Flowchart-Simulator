#ifndef CONNECTOR_H
#define CONNECTOR_H

#include<fstream>
#include "..\GUI\UI_Info.h"
#include "..\GUI\output.h"

class Statement;

class Connector	//a connector that connects two statements (Source & Destination)
{
private:
	Statement *SrcStat;	//The source statement of the connector
	Statement *DstStat;	//The destination statement of the connector
	Point Start;	//Start point of the connector
	Point End;		//End point of the connector

	pair<Point, Point> Segments[10000]; // Segments of the connector
	int SegCount;
	bool Selected;
	string state;


public:
	Connector(Statement* Src, Statement* Dst);

	void		setSrcStat(Statement *Src);
	Statement*	getSrcStat();	
	void		setDstStat(Statement *Dst);
	Statement*	getDstStat();

	void setStartPoint(Point P);
	Point getStartPoint();

	void setEndPoint(Point P);
	Point getEndPoint();

	void Draw(Output* pOut, bool Clear = false);

	int GetSegCount() const;
	void GetSegments(pair<Point, Point> Seg[]) const;
	void SetSegments(const pair<Point, Point> Seg[]);

	bool IsSelected() const;
	void SetSelected(bool s);

	void setState(string s);

};

#endif