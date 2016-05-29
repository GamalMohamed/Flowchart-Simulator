#include "ApplicationManager.h"


int main()
{
	ActionType ActType;
	ApplicationManager AppManager;  //Create an object of ApplicationManager

	do
	{
		//Step I - Read user input (action)
		ActType = AppManager.GetUserAction();

		//Steps II & III - Create & Exexute the action
		AppManager.ExecuteAction(ActType);

		//Step IV - Update the interface
		AppManager.UpdateInterface();

	} while (ActType != EXIT);

	return 0;
}

