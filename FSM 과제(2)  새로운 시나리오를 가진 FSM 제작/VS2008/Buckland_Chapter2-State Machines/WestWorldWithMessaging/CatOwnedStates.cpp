#include "Cat.h"
#include "CatOwnedStates.h"
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
CatGlobalState* CatGlobalState::Instance()
{
	static CatGlobalState instance;

	return &instance;
}


void CatGlobalState::Execute(Cat* cat)
{
	//1 in 10 chance of needing the bathroom (provided she is not already
	//in the bathroom)

}

bool CatGlobalState::OnMessage(Cat* cat, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{


		return true;

	}//end switch

	return false;
}

#pragma endregion

#pragma region ½Ä»§ÀÚ¼¼
Bread_posture* Bread_posture::Instance()
{
	static Bread_posture instance;

	return &instance;
}

void Bread_posture::Enter(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : ½Ä»§ ±¸¿ö¾ßÁö";
}

void Bread_posture::Execute(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : °ñ°ñ°ñ";

	int random = rand() % 10;
	if (random <= 1)
	{		
		cat->GetFSM()->ChangeState(Break::Instance());
	}
}

void Bread_posture::Exit(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : ½Ä»§ ¿Ï¼º";
}

bool Bread_posture::OnMessage(Cat* cat, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{
	case Msg_Eat:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(cat->ID()) << " at time: "
			<< Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(cat->ID()) <<
			" : ÀÌ ³¿»õ´Â? Ãò¸£¼Ò¸®";
		
		cat->GetFSM()->ChangeState(Eat::Instance());
	}
		return true;

	}//end switch

	return false;
}
#pragma endregion

#pragma region ¹ä¸Ô±â
Eat* Eat::Instance()
{
	static Eat instance;

	return &instance;
}

void Eat::Enter(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : Ãò¸£´Ù";
}

void Eat::Execute(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : ³È³È³È";
	cat->GetFSM()->ChangeState(Bread_posture::Instance());
}

void Eat::Exit(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : Àß¸Ô¾ú´Ù";
}

bool Eat::OnMessage(Cat* cat, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{

		return true;

	}//end switch

	return false;
}
#pragma endregion

#pragma region ¹°°ÇƒÆ±â
Break* Break::Instance()
{
	static Break instance;

	return &instance;
}

void Break::Enter(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : Áý»ç¸¦ Èûµé°Ô ÇØº¼±î?";
}

void Break::Execute(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : ¤ŸÅ©¶û";
	//Áý»çÇÑÅ× ¹°°ÇÀ» ²£´Ù°í Àü¼Û
	Dispatch->DispatchMessage(0.1,                  //time delay
		cat->ID(),           //sender ID
		ent_Cat_Master,           //receiver ID
		Msg_Break,        //msg  Msg_Deal
		NO_ADDITIONAL_INFO);
	cat->GetFSM()->ChangeState(Bread_posture::Instance());

}

void Break::Exit(Cat* cat)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(cat->ID()) <<
		" : È÷È÷ ½Ä»§ ¸¸µé·¯ °¡¾ßÁö";
}

bool Break::OnMessage(Cat* cat, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{

		return true;

	}//end switch

	return false;
}
#pragma endregion