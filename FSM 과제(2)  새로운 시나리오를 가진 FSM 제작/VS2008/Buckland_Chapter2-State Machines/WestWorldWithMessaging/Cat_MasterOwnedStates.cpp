#include "Cat_MasterOwnedStates.h"
#include "Cat_Master.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

#pragma region Global state
Cat_MasterGlobalState* Cat_MasterGlobalState::Instance()
{
	static Cat_MasterGlobalState instance;

	return &instance;
}


void Cat_MasterGlobalState::Execute(Cat_Master* catMaster)
{
	//1 in 10 chance of needing the bathroom (provided she is not already
	//in the bathroom)

}

bool Cat_MasterGlobalState::OnMessage(Cat_Master* catMaster, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	

	return true;

	}//end switch

	return false;
}

#pragma endregion

#pragma region ���ϱ�
DoWork* DoWork::Instance()
{
	static DoWork instance;

	return &instance;
}

void DoWork::Enter(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	
}

void DoWork::Execute(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);	
}

void DoWork::Exit(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	
}

bool DoWork::OnMessage(Cat_Master* catMaster, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{
	
	return true;

	}//end switch

	return false;
}
#pragma endregion

#pragma region ���ֱ�
Feeding* Feeding::Instance()
{
	static Feeding instance;

	return &instance;
}

void Feeding::Enter(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

}

void Feeding::Execute(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void Feeding::Exit(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

}

bool Feeding::OnMessage(Cat_Master* catMaster, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{

		return true;

	}//end switch

	return false;
}
#pragma endregion

#pragma region û���ϱ�
Cleaning* Cleaning::Instance()
{
	static Cleaning instance;

	return &instance;
}

void Cleaning::Enter(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

}

void Cleaning::Execute(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void Cleaning::Exit(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

}

bool Cleaning::OnMessage(Cat_Master* catMaster, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{

		return true;

	}//end switch

	return false;
}
#pragma endregion
