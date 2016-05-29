#include "Input.h"
#include "Output.h"


Input::Input(window* pW)
{
	pWind = pW; //point to the passed window
}

void Input::GetPointClicked(Point &P) const
{
	pWind->WaitMouseClick(P.x, P.y);	//Wait for mouse click
}

keytype Input::Getkeypress()
{
	char cKeyData;
	return pWind->GetKeyPress(cKeyData);
}

buttonstate Input::Getbuttonstate(button b, int &iX, int &iY)
{
	return pWind->GetButtonState(b, iX, iY);
}

string Input::GetString(Output *pO) const
{
	pWind->FlushKeyQueue();

	string Str;
	char Key;

	while (1)
	{
		pWind->WaitKeyPress(Key);

		if (Key == 27)	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label

		else if (Key == 13) //ENTER key is pressed
		{
			if (Str.length() > 50)
			{
				pO->PrintMessage("Text size limit exceeded !! Re-Enter a text less than 50 characters");
				Str.clear();
			}
			else if (Str.length() <= 50)
				return Str;
		}
		else if (Key == 8) //BackSpace is pressed
		{
			if (Str.size() != 0)
			{
				Str.resize(Str.size() - 1);
				pO->PrintMessage(Str);
			}
		}

		//not arrows or f3 or f11 or f12 or pgdwn or home
		else if (Key != 2 && Key != 4 && Key != 6 && Key != 3 && Key != 9 && Key != 7 && Key != 11 && Key != 12)
		{
			Str += Key;
			pO->PrintMessage(Str);
		}
	}
}

string Input::GetValue(Output* pO) const	// Reads a double value from the user 
{
	//Read a value from the user
	string Str;
	char Key;
	int Once, Beg;
	Once = Beg = 0;

	for (int i = 0;;)
	{
		pWind->WaitKeyPress(Key);

		if (Key == 27)	 //ESCAPE key is pressed
			return 0;	//returns nothing as user has cancelled label

		else if (Key == 13)	//ENTER key is pressed
			return Str;

		else if (Key == 8) //BackSpace is pressed
		{
			if (Str.size() != 0)
			{
				if (Str[i - 1] == '.')
					Once = 0;

				if (Str[i - 1] == '-')
					Beg = 0;

				Str.resize(Str.size() - 1);
				i--;
			}
		}

		else if (Key == 45) //minus pressed
		{
			if (Beg == 0 && !i)
			{
				Str += Key;
				Beg++;
				i++;
			}

		}

		else if (Key == 46) //point pressed
		{
			if (Once == 0)
			{
				Str += Key;
				Once++;
				i++;
			}
		}

		else if (Key >= 48 && Key <= 57) //numbers pressed
		{
			Str += Key;
			i++;
		}

		pO->PrintMessage(Str);
	}
}

ActionType Input::GetUserAction(int &x, int &y) const
{
	//This function reads the position where the user clicks to determine the desired action
	pWind->FlushMouseQueue();

	pWind->WaitMouseClick(x, y);	//Get the coordinates of the user click

	if (UI.AppMode == DESIGN)	//Application is in Design mode
	{
		//[1] If user clicks on the Toolbar
		if (y >= 0 && y <= UI.TlBrWdth / 2)
		{
			if (x >= 0 && x <= 64)
				return LOAD;
			if (x >= 64 && x <= 120)
				return SAVE;

			if (x >= 130 && x <= 180)
				return UNDO;
			if (x >= 180 && x <= 230)
				return REDO;

			if (x >= 235 && x <= 290)
				return COPY;
			if (x >= 290 && x <= 335)
				return CUT;
			if (x >= 335 && x <= 385)
				return PASTE;
			if (x >= 385 && x <= 435)
				return DEL;

			if (x >= 450 && x <= 500)
				return ZOOM_IN;
			if (x >= 500 && x <= 550)
				return ZOOM_OUT;
			if (x >= 550 && x <= 600)
				return MOVE;

			if (x >= 860 && x <= 920)
				return SIM_MODE;

			if (x >= 920 && x <= 975)
				return MINIMIZE;
			if (x >= 975 && x <= 1040)
				return EXIT;
		}

		else if (y >= UI.TlBrWdth / 2 && y <= UI.TlBrWdth)
		{
			if (x >= 0 && x <= 95)
				return START;
			if (x >= 95 && x <= 175)
				return END;
			if (x >= 175 && x <= 275)
				return READ;
			if (x >= 275 && x <= 370)
				return WRITE;
			if (x >= 370 && x <= 470)
				return ADD_ASSIGN;
			if (x >= 470 && x <= 615)
				return ADD_CONDITION;
			if (x >= 615 && x <= 740)
				return ADD_CONNECTOR;
			if (x >= 740 && x <= 860)
				return COMMENT;
			if (x >= 860 && x <= 940)
				return EDIT_CONNECTOR;
			if (x >= 940 && x <= 1040)
				return EDIT_STAT;
		}

		//[2] User clicks on the drawing area
		if (y >= UI.TlBrWdth && y <= UI.height - UI.StBrWdth)
			return SELECT;	//user want to select/unselect a statement in the flowchart

		//[3] User clicks on the status bar or on an empty place in a toolbar
		return STATUS;
	}

	else	//Application is in Simulation mode
	{
		//[1] If user clicks on the Toolbar
		if (y >= 0 && y <= UI.TlBrWdth / 2)
		{
			if (x >= 0 && x <= 75)
				return VALIDATE;
			if (x >= 75 && x <= 135)
				return RUN;
			if (x >= 135 && x <= 195)
				return DEBUG;
			if (x >= 195 && x <= 255)
				return GENERATE_CODE;

			if (x >= 860 && x <= 920)
				return DSN_MODE;

			if (x >= 920 && x <= 975)
				return MINIMIZE;
			if (x >= 975 && x <= 1040)
				return EXIT;
		}
		//[2] User clicks on the status bar or on an empty place in a toolbar
		return STATUS;
	}
}

Input::~Input()
{
}