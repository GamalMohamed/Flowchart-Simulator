#include "Connector.h"

Connector::Connector(Statement* Src, Statement* Dst)	
//When a connector is created, it must have a source statement and a destination statement
//There are no free connectors in the folwchart
{
	SrcStat = Src;
	DstStat = Dst;
	SegCount = 0;
	Selected = false;
}

Statement* Connector::getSrcStat()
{	return SrcStat;	}

Statement* Connector::getDstStat()
{	return DstStat;	}

void Connector::setSrcStat(Statement *Src)
{	SrcStat = Src;	}

void Connector::setDstStat(Statement *Dst)
{	DstStat = Dst;	}

void Connector::setStartPoint(Point P)
{	Start = P;	}

Point Connector::getStartPoint()
{	return Start;	}

void Connector::setEndPoint(Point P)
{	End = P;	}

Point Connector::getEndPoint()
{	return End;	}

int Connector::GetSegCount() const
{	return SegCount;	}

void Connector::GetSegments(pair<Point, Point> Seg[]) const
{
	for (int i = 0; i < SegCount; ++i)
	{
		Seg[i] = Segments[i];
	}
}

void Connector::SetSegments(const pair<Point, Point> Seg[])
{
	for (int i = 0; i < SegCount; ++i)
	{
		Segments[i] = Seg[i];
	}
}

bool Connector::IsSelected() const
{
	return Selected;
}

void Connector::SetSelected(bool s)
{
	Selected = s;
}

void Connector::Draw(Output* pOut, bool Clear)
{
	///TODO: Call output to draw a connector from SrcStat to DstStat on the output window
	pOut->DrawConnector(Start, End, Segments, SegCount, Clear, Selected);
}

void Connector::setState(string s)
{
	state = s; 
}
