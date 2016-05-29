#include "Output.h"
#include "../ApplicationManager.h"


Output::Output(ApplicationManager *pAppManager)
{
	pManager = pAppManager;

	//Initialize user interface parameters
	UI.width = 1040;
	UI.height = 700;
	UI.wx = 0;
	UI.wy = 0;

	UI.AppMode = DESIGN;	//Design Mode is the default mode

	UI.MnItWdth = 0;
	UI.MnItHeight = 120;
	UI.TlBrWdth = 120;
	UI.StBrWdth = 62;

	UI.DrawClr = BLUE;
	UI.HiClr = RED;
	UI.MsgClr = RED;

	UI.ASSGN_WDTH = 150;
	UI.ASSGN_HI = 50;

	UI.START_WDTH = 150;
	UI.START_HI = 50;

	UI.END_WDTH = 150;
	UI.END_HI = 50;

	UI.READ_WDTH = 150;
	UI.READ_HI = 50;

	UI.WRITE_WDTH = 150;
	UI.WRITE_HI = 50;

	UI.COND_WDTH = 100;
	UI.COND_HI = 100;

	//Create the output window
	pWind = CreateWind(UI.width, UI.height, UI.wx, UI.wy);
	//Change the title
	pWind->ChangeTitle("Programming Techniques Project");

	color background(120, 120, 120);
	pWind->SetBrush(background);
	pWind->DrawRectangle(0, 0, UI.width, UI.height);

	pWind->SetPen(BLUE, 50);
	pWind->SetFont(45, BOLD, BY_NAME, "Times New Roman");
	pWind->DrawString(70, 260, "Welcome to <Flowchart> Simple Simulating system");

	pWind->SetPen(RED, 50);
	pWind->SetFont(28, BOLD, BY_NAME, "Arial");
	pWind->DrawString(340, 600, "Click anywhere to Start...");

	int x, y; x = y = 0;
	pWind->WaitMouseClick(x, y);

	pWind->SetPen(WHITE, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, 0, UI.width, UI.height);

	CreateDesignToolBar();
	CreateStatusBar();

	//CreateMainMenu(pWind->GetHWind());
}

Input* Output::CreateInput()
{
	Input* pIn = new Input(pWind);
	return pIn;
}

//======================================================================================//
//								Interface Functions										//
//======================================================================================//

window* Output::CreateWind(int wd, int h, int x, int y)
{
	return new window(wd, h, x, y);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::CreateStatusBar()
{
	string s = "images\\Status.jpg";
	pWind->DrawImage(s, 0, UI.height - UI.StBrWdth);
}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the Design Menu
void Output::CreateDesignToolBar()
{
	UI.AppMode = DESIGN;	//Design Mode

	//fill the tool bar 
	string s = "images\\Design.jpg";
	pWind->DrawImage(s, 0, 0);

	//set draw area region
	ClearDrawArea();
}

void Output::CreateSimulationToolBar()
{
	ClearAll();
	UI.AppMode = SIMULATION;	//Simulation Mode

	//fill the tool bar 
	string s = "images\\Simulation.jpg";
	pWind->DrawImage(s, 0, 0);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::ClearStatusBar()
{
	CreateStatusBar();
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::ClearDrawArea()
{
	pWind->SetPen(WHITE, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, UI.TlBrWdth, UI.width, UI.height - UI.StBrWdth);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::ClearAll()
{
	pWind->SetPen(WHITE, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, 0, UI.width, UI.height - UI.StBrWdth);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Output::ClearStat(Point p, char w, int W, int H, bool z)
{
	pWind->SetPen(WHITE, 3);
	pWind->SetBrush(WHITE);

	if (w == 'S')
		pWind->DrawRectangle(p.x - W / 2 - 2, p.y - 2, p.x + W / 2 + 2, p.y + H + 2);
	/*pWind->DrawEllipse(p.x - UI.START_WDTH / 2 ,
	p.y , p.x + UI.START_WDTH / 2 , p.y + UI.START_HI );*/

	else if (w == 'W' || w == 'R')
	{
		/*int pX[] = { p.x - UI.READ_WDTH / 2, p.x + UI.READ_WDTH / 2, p.x + UI.READ_WDTH / 4, p.x - 3 * UI.READ_WDTH / 4 };
		int pY[] = { p.y, p.y, p.y + UI.READ_HI, p.y + UI.READ_HI };
		pWind->DrawPolygon(pX, pY, 4);*/

		pWind->DrawRectangle(p.x - 3 * W / 4 - 2, p.y - 2, p.x + W / 2 + 2, p.y + H + 2);
	}

	else if (w == 'C')
	{
		if (!z)
			pWind->DrawRectangle(p.x - W / 2 - 42, p.y - 2, p.x + W / 2 + 42, p.y + H + 2);
		else
			pWind->DrawRectangle(p.x - W / 2 - 82, p.y - 2, p.x + W / 2 + 82, p.y + H + 2);
		/*int pX[] = { p.x, p.x + UI.COND_WDTH / 2, p.x, p.x - UI.COND_WDTH / 2};

		int pY[] = { p.y, p.y + UI.COND_HI / 2, p.y + UI.COND_HI, p.y + UI.COND_HI / 2 };

		pWind->DrawPolygon(pX, pY, 4);

		pWind->DrawLine(pX[1], pY[1], pX[1] + 40, pY[1]);

		pWind->DrawLine(pX[3] - 40, pY[3], pX[3], pY[3]);


		pWind->DrawString(pX[1], pY[1] - 20, "Yes");
		pWind->DrawString(pX[3] - 20, pY[3] - 20, "No");*/
	}

	else if (w == 'A')
		pWind->DrawRectangle(p.x - W / 2, p.y, p.x + W / 2, p.y + H);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Output::PrintMessage(string msg)	//Prints a message on status bar
{
	ClearStatusBar();	//First clear the status bar
	
	pWind->SetPen(UI.MsgClr, 50);
	pWind->SetFont(20, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, UI.height - (int)(UI.StBrWdth / 1.3), msg);
}

void Output::MsgBox(string message, string caption, bool isError) const
{
	UINT uType;
	if (isError)
		uType = MB_ICONERROR;
	else
		uType = MB_ICONINFORMATION;
	MessageBox(GetWind()->GetHWind(), message.c_str(), caption.c_str(), uType);
}

//======================================================================================//
//								Statements Drawing Functions								//
//======================================================================================//

//Draw assignment statement and write the "Text" on it
void Output::DrawAssign(Point TopCenter, int &width, int &height, string Text, bool Selected, bool Z)
{
	if (Z)
		pWind->SetFont(60, BOLD, BY_NAME, "Arial");
	else
		pWind->SetFont(25, BOLD, BY_NAME, "Arial");


	int text_width, text_height;
	pWind->GetStringSize(text_width, text_height, Text);

	if (text_width >= width)
		width += ((text_width - UI.ASSGN_WDTH) + 10);

	if (Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr, 3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr, 3);	//use normal color

	//Draw the statement block rectangle
	pWind->DrawRectangle(TopCenter.x - width / 2, TopCenter.y, TopCenter.x + width / 2, TopCenter.y + height);

	//Write statement text
	pWind->SetPen(BLACK, 2);

	pWind->DrawString(TopCenter.x - text_width / 2, TopCenter.y + height / 2 - text_height / 2, Text);
}

void Output::DrawStart(Point TopCenter, int width, int height, string Text, bool Selected, bool Z)
{
	if (Z)
		pWind->SetFont(60, BOLD, BY_NAME, "Arial");
	else
		pWind->SetFont(25, BOLD, BY_NAME, "Arial");

	if (Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr, 3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr, 3);	//use normal color

	//Draw the statement block rectangle
	pWind->DrawEllipse(TopCenter.x - width / 2, TopCenter.y, TopCenter.x + width / 2, TopCenter.y + height);

	//Write statement text
	pWind->SetPen(BLACK, 2);

	int text_width, text_height;
	pWind->GetStringSize(text_width, text_height, Text);

	pWind->DrawString(TopCenter.x - text_width / 2, TopCenter.y + height / 2 - text_height / 2, Text);

}

void Output::DrawEnd(Point TopCenter, int width, int height, string text, bool Selected, bool Z)
{
	if (Z)
		pWind->SetFont(60, BOLD, BY_NAME, "Arial");
	else
		pWind->SetFont(25, BOLD, BY_NAME, "Arial");



	if (Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr, 3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr, 3);	//use normal color

	//Draw the statement block rectangle
	pWind->DrawEllipse(TopCenter.x - width / 2, TopCenter.y, TopCenter.x + width / 2, TopCenter.y + height);

	//Write statement text
	pWind->SetPen(BLACK, 2);

	int text_width, text_height;
	pWind->GetStringSize(text_width, text_height, text);

	pWind->DrawString(TopCenter.x - text_width / 2, TopCenter.y + height / 2 - text_height / 2, text);
}

void Output::DrawRead(Point TopCenter, int &width, int &height, string Text, bool Selected, bool Z)
{
	if (Z)
		pWind->SetFont(60, BOLD, BY_NAME, "Arial");
	else
		pWind->SetFont(25, BOLD, BY_NAME, "Arial");


	int text_width, text_height;
	pWind->GetStringSize(text_width, text_height, Text);

	if (text_width >= width)
		width += (text_width);
	
	if (Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr, 3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr, 3);	//use normal color

	//Draw the statement block rectangle

	int pX[] = { TopCenter.x - width / 2, TopCenter.x + width / 2, TopCenter.x + width / 4, TopCenter.x - 3 * width / 4 };

	int pY[] = { TopCenter.y, TopCenter.y, TopCenter.y + height, TopCenter.y + height };

	pWind->DrawPolygon(pX, pY, 4);

	//Write statement text
	pWind->SetPen(BLACK, 2);

	pWind->DrawString(TopCenter.x - 3 * text_width / 4, TopCenter.y + height / 2 - text_height / 2, Text);

}

void Output::DrawWrite(Point TopCenter, int &width, int &height, string Text, bool Selected, bool Z)
{
	if (Z)
		pWind->SetFont(60, BOLD, BY_NAME, "Arial");
	else
		pWind->SetFont(25, BOLD, BY_NAME, "Arial");


	int text_width, text_height;
	pWind->GetStringSize(text_width, text_height, Text);

	if (text_width >= width)
		width += (text_width);

	if (Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr, 3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr, 3);	//use normal color

	//Draw the statement block rectangle

	int pX[] = { TopCenter.x - width / 2, TopCenter.x + width / 2, TopCenter.x + width / 4, TopCenter.x - 3 * width / 4 };

	int pY[] = { TopCenter.y, TopCenter.y, TopCenter.y + height, TopCenter.y + height };

	pWind->DrawPolygon(pX, pY, 4);

	//Write statement text
	pWind->SetPen(BLACK, 2);



	pWind->DrawString(TopCenter.x - 3 * text_width / 4, TopCenter.y + height / 2 - text_height / 2, Text);
}

void Output::DrawCondtional(Point TopCenter, int &width, int &height, string Text, bool Selected, bool Z)
{
	if (Z)
		pWind->SetFont(60, BOLD, BY_NAME, "Arial");
	else
		pWind->SetFont(25, BOLD, BY_NAME, "Arial");

	int text_width, text_height;
	pWind->GetStringSize(text_width, text_height, Text);

	if (text_width >= width)
		width += (text_width);

	if (Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr, 3);	//use highlighting color
	else
		pWind->SetPen(UI.DrawClr, 3);	//use normal color

	//Draw the statement block rectangle

	int pX[] = { TopCenter.x, TopCenter.x + width / 2, TopCenter.x, TopCenter.x - width / 2 };

	int pY[] = { TopCenter.y, TopCenter.y + height / 2, TopCenter.y + height, TopCenter.y + height / 2 };

	pWind->DrawPolygon(pX, pY, 4);

	// Write statement text

	if (!Z)
	{

		pWind->DrawLine(pX[1], pY[1], pX[1] + 40, pY[1]);

		pWind->DrawLine(pX[3] - 40, pY[3], pX[3], pY[3]);

		pWind->SetPen(BLACK, 3);
		pWind->DrawString(TopCenter.x - text_width / 2, TopCenter.y + height / 2 - text_height / 2, Text);

		pWind->SetFont(20, BOLD, BY_NAME, "Arial");
		pWind->DrawString(pX[1], pY[1] - 20, "Yes");
		pWind->DrawString(pX[3] - 20, pY[3] - 20, "No");
	}
	else
	{
		pWind->DrawLine(pX[1], pY[1], pX[1] + 80, pY[1]);
		pWind->DrawLine(pX[3] - 80, pY[3], pX[3], pY[3]);

		pWind->SetPen(BLACK, 3);

		pWind->SetFont(50, BOLD, BY_NAME, "Arial");
		pWind->DrawString(TopCenter.x - text_width / 2, TopCenter.y + height / 2 - text_height / 2, Text);

		pWind->SetFont(40, BOLD, BY_NAME, "Arial");
		pWind->DrawString(pX[1], pY[1] - 40, "Yes");
		pWind->DrawString(pX[3] - 40, pY[3] - 40, "No");
	}

}

void Output::DrawConnector(Point From, Point To, pair<Point, Point> Segments[], int &SegCount, bool Clear, bool Selected)
{
	SegCount = 0;
	Point P1, P2;

	if (Selected)	//if stat is selected, it should be highlighted
		pWind->SetPen(UI.HiClr, 3);	//use highlighting color
	else if (Clear)
		pWind->SetPen(WHITE, 3);	//use white color to clear
	else
		pWind->SetPen(UI.DrawClr, 3);	//use normal color

	int segment = 10;

	// Draw Arrow
	P1.x = To.x;
	P1.y = To.y;
	P2.x = To.x + segment;
	P2.y = To.y - segment;
	pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
	Segments[SegCount++] = { P1, P2 };

	P1.x = To.x;
	P1.y = To.y;
	P2.x = To.x - segment;
	P2.y = To.y - segment;
	pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
	Segments[SegCount++] = { P1, P2 };

	P1.x = To.x;
	P1.y = To.y;
	P2.x = To.x;
	P2.y = To.y - 2 * segment;
	pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
	Segments[SegCount++] = { P1, P2 };
	To.y -= 2 * segment;

	bool isCond = !pManager->GetStatement(From);

	// Draw remaining part segment by segment (segment length = 10 px)
	if (!isCond)
	{
		P1.x = From.x;
		P1.y = From.y;
		P2.x = From.x;
		P2.y = From.y + segment;
		pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
		Segments[SegCount++] = { P1, P2 };
		From.y += segment;
	}
	Point Current = From, Next = From;
	if (From.y <= To.y) {
		while (To.y - Current.y > segment)
		{
			Next.y += 2 * segment;
			Statement* st = pManager->GetStatement(Next);
			if (st != NULL)
			{
				if (st->getRightEdge() - Current.x <= Current.x - st->getLeftEdge())
					Next.x = st->getRightEdge() + segment;
				else
					Next.x = st->getLeftEdge() - segment;
				Next.y -= segment;
			}
			Next.y -= segment;
			P1.x = Current.x;
			P1.y = Current.y;
			P2.x = Next.x;
			P2.y = Next.y;
			pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
			Segments[SegCount++] = { P1, P2 };
			Current = Next;
		}
		Next.y = To.y;
		P1.x = Current.x;
		P1.y = Current.y;
		P2.x = Current.x;
		P2.y = Next.y;
		pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
		Segments[SegCount++] = { P1, P2 };
		Current = Next;
		P1.x = Current.x;
		P1.y = Current.y;
		P2.x = To.x;
		P2.y = To.y;
		pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
		Segments[SegCount++] = { P1, P2 };
	}
	else {
		if (!isCond)
		{
			Next.x += UI.ASSGN_WDTH / 2 + 2 * segment;
			P1.x = Current.x;
			P1.y = Current.y;
			P2.x = Next.x;
			P2.y = Next.y;
			pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
			Segments[SegCount++] = { P1, P2 };
		}
		Current = Next;
		while (Current.y - To.y > segment)
		{
			Next.y -= 2 * segment;
			Statement* st = pManager->GetStatement(Next);
			if (st != NULL)
			{
				if (st->getRightEdge() - Current.x <= Current.x - st->getLeftEdge())
					Next.x = st->getRightEdge() + segment;
				else
					Next.x = st->getLeftEdge() - segment;
				Next.y += segment;
			}
			Next.y += segment;
			P1.x = Current.x;
			P1.y = Current.y;
			P2.x = Next.x;
			P2.y = Next.y;
			pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
			Segments[SegCount++] = { P1, P2 };
			Current = Next;
		}
		Next.y = To.y;
		P1.x = Current.x;
		P1.y = Current.y;
		P2.x = Current.x;
		P2.y = Next.y;
		pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
		Segments[SegCount++] = { P1, P2 };
		Current = Next;
		P1.x = Current.x;
		P1.y = Current.y;
		P2.x = To.x;
		P2.y = To.y;
		pWind->DrawLine(P1.x, P1.y, P2.x, P2.y);
		Segments[SegCount++] = { P1, P2 };
	}
}

color Output::GetColor(Point Pos) const
{
	return pWind->GetColor(Pos.x, Pos.y);
}


bool Output::isDrawColor(color c) const
{
	return c.ucRed == UI.DrawClr.ucRed && c.ucGreen == UI.DrawClr.ucGreen && c.ucBlue == UI.DrawClr.ucBlue;
}

void Output::CreateMainMenu(HWND hWnd)
{
	HMENU hMenu = CreateMenu();
	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&About");
	AppendMenu(hSubMenu, MF_STRING, 1, "&About...");
	SetMenu(hWnd, hMenu);
}

Output::~Output()
{
	delete pWind;
}



