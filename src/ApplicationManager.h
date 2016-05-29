#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H


#include "DEFS.h"
#include "GUI\Input.h"
#include "GUI\Output.h"
#include "Statements\Statement.h"
#include "Actions\AddAssign.h"
#include "Actions\AddStart.h"
#include "Actions\AddEnd.h"
#include "Actions\AddWrite.h"
#include "Actions\AddRead.h"
#include "Actions\AddCond.h"
#include "Actions\Copy.h"
#include "Actions\Cut.h"
#include "Actions\Paste.h"
#include "Actions\Move.h"
#include "Actions\AddConnector.h"
#include "Actions\EditConnector.h"
#include "Actions\Save.h"
#include "Actions\Load.h"
#include "Actions\Select.h"
#include "Actions\Comment.h"
#include "Actions\EditStat.h"
#include "Actions\Delete.h"
#include "Actions\Validate.h"
#include "Actions\Run.h"
#include "Actions\Debug.h"
#include "Actions\GenerateCode.h"
#include "Actions\ZoomIN.h"
#include "Actions\ZoomOUT.h"

class Input;
class Output;



//Main class that manages everything in the application.
class ApplicationManager
{
	friend class Copy;
	friend class Cut;
	friend class Paste;
	friend class Move;
	friend class Save;
	friend class Load;
	friend class EditConnector;
	friend class DeleteConnector;
	friend class Select;
	friend class Comment;
	friend class EditStat;
	friend class Delete;
	friend class Validate;
	friend class Run;
	friend class Debug;
	friend class GenerateCode;
	friend class ZoomIN;
	friend class ZoomOUT;

	enum { MaxCount = 200 };	//Max no of statements/connectors in a single flowchart
	
private:
	int StatCount;		                      //Actual number of statements
	int ConnCount;		                     //Actual number of connectors
 	int SelectedCount;                      //Actual number of Selected statements
	int ConnSelectedCount;					//Actual number of Selected connectors
	int nS, nE;                            //number of Start and End statements in the whole flowchart
	Point p;
	bool ZM; //Zoom mode 
	//Statement* StatList[MaxCount];	      //List of all statements (Array of pointers)
	//Connector* ConnList[MaxCount];	     //List of all connectors (Array of pointers)
	Statement** StatList = new Statement*[MaxCount];	      //List of all statements (Array of pointers)
	Connector** ConnList = new Connector*[MaxCount];	     //List of all connectors (Array of pointers)
	Statement** SelectedList = new Statement*[MaxCount];	//List of all Selected statements (Array of pointers)
	Connector** ConnSelectedList = new Connector*[MaxCount];	//List of all Selected connectors (Array of pointers)



	Statement *pSelectedStat; //a pointer to the last selected statement

	//Pointers to Input and Output classes
	Input *pIn;
	Output *pOut;

public:	
	ApplicationManager(); 
	~ApplicationManager();

	int getStatCount();
	int getConnCount();
	int getSelectedCount();
	int getConnSelectedCount();
	void setZoomMode(bool);
	bool getZoomMode();

	

	//// -- Actions Related Functions
	//Reads the input command from the user and returns the corresponding action type
	ActionType GetUserAction();
	void ExecuteAction(ActionType) ; //Creates an action and executes it
	

	//// -- Statements/Connector Management Functions
	void AddStatement(Statement* pStat, bool = false);     //Adds a new Statement to the Flowchart
	Statement *GetStatement(Point P) const;	//search for a statement where point P belongs


	void SetSelectedStatement(Statement *pStat); //Set the Statement selected by the user
	Statement *GetSelectedStatement() const;	//Returns the selected Statement
	Statement** GetAllSelected() const;
	Connector** GetAllSelectedConn() const;


	
	void AddSelected(Statement* pStat); //Adds a new selected Statement to the list
	void RemoveSelected(Statement* pStat);// Remove item from selection
	void EmptySelectedList();

	void AddSelectedConn(Connector* pConn); //Adds a new selected connector to the list
	void RemoveSelectedConn(Connector* pConn);// Remove connector from selection
	void EmptyConnSelectedList();
	void EmptyStatList();
	void EmptyConnList();
	void SelectALL();




	void AddConn(Connector* pConn); //Adds a new Connector to the Flowchart
	Connector *GetConnector(Point P) const;	//search for a Connector where point P belongs

	
	//// -- Interface Management Functions
	Input *GetInput() const; //Return pointer to the input
	Output *GetOutput() const; //Return pointer to the output
	void UpdateInterface() const;	//Redraws all the drawing window
	
	
};

#endif