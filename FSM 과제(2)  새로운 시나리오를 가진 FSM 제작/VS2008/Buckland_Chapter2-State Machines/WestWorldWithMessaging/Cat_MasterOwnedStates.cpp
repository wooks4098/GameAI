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

#pragma region 일하기
DoWork* DoWork::Instance()
{
	static DoWork instance;

	return &instance;
}

void DoWork::Enter(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 일을 해볼까?";
}

void DoWork::Execute(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);	
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 영차 영차";
	int random = rand() % 10;
	if (random <= 1)
	{
		catMaster->GetFSM()->ChangeState(Feeding::Instance());
	}
}

void DoWork::Exit(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 후.. 힘들다 그만해야지";
	
}

bool DoWork::OnMessage(Cat_Master* catMaster, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{
	case Msg_Break:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(catMaster->ID()) << " at time: "
			<< Clock->GetCurrentTime();


		catMaster->GetFSM()->ChangeState(Cleaning::Instance());
	}
	return true;

	}//end switch

	return false;
}
#pragma endregion

#pragma region 밥주기
Feeding* Feeding::Instance()
{
	static Feeding instance;

	return &instance;
}

void Feeding::Enter(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 츄르를 줄까?";
}

void Feeding::Execute(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 츄르 먹자";
	Dispatch->DispatchMessage(0.1,                  //time delay
		catMaster->ID(),           //sender ID
		ent_Cat,           //receiver ID
		Msg_Eat,        //msg  Msg_Deal
		NO_ADDITIONAL_INFO);
	catMaster->GetFSM()->ChangeState(DoWork::Instance());

}

void Feeding::Exit(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

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

#pragma region 청소하기
Cleaning* Cleaning::Instance()
{
	static Cleaning instance;

	return &instance;
}

void Cleaning::Enter(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 이게 무슨일이야";

}

void Cleaning::Execute(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 어휴 어휴";
	catMaster->GetFSM()->ChangeState(DoWork::Instance());
}

void Cleaning::Exit(Cat_Master* catMaster)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(catMaster->ID()) <<
		" : 다치웠다!!!";

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
