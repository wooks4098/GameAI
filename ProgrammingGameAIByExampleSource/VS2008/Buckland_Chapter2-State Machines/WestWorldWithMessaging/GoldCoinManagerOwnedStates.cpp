#include "GoldCoinManagerOwnedStates.h"
#include "MinerOwnedStates.h"
#include "GoldCoinManager.h"
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

//-----------------------------------------------------------------------Global state

GoldCoinManagerGlobalState* GoldCoinManagerGlobalState::Instance()
{
	static GoldCoinManagerGlobalState instance;

	return &instance;
}


void GoldCoinManagerGlobalState::Execute(GoldCoinManager* CoinManager)
{
	//1 in 10 chance of needing the bathroom (provided she is not already
	//in the bathroom)
	if ((RandFloat() < 0.1) &&
		!CoinManager->GetFSM()->isInState(*DrinkCoffe::Instance()))
	{
		CoinManager->GetFSM()->ChangeState(DrinkCoffe::Instance());
	}
}

bool GoldCoinManagerGlobalState::OnMessage(GoldCoinManager* CoinManager, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_GoBank:
	{
		cout << "\nMessage handled by " << GetNameOfEntity(CoinManager->ID()) << " at time: "
			<< Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(CoinManager->ID()) <<
			"손님 좋은 상품이 있습니다. 돈이 복사가 된다구요!!";

		CoinManager->GetFSM()->ChangeState(SellCoin::Instance());
	}

	return true;

	}//end switch

	return false;
}

//-------------------------------------------------------------------------DoHouseWork

DoBankWork* DoBankWork::Instance()
{
	static DoBankWork instance;

	return &instance;
}


void DoBankWork::Enter(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 코인 변동률을 확인할 시간이다";
}


void DoBankWork::Execute(GoldCoinManager* CoinManager)
{
	int random = RandInt(0, 9);

	if (random >= 7) //코인 상승
	{
		CoinManager->Set_Coin_Pirce(+1);
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 코인 가격 상승	(1Coin = " + std::to_string(CoinManager->Return_Coin_Pirce()) + "금덩이)";
	}
	else if (random >= 3 && random <= 6 && CoinManager->Return_Coin_Pirce() >= 2) //코인 하락
	{
		CoinManager->Set_Coin_Pirce(-1);
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 코인 가격 하락	(1Coin = " + std::to_string(CoinManager->Return_Coin_Pirce()) + "금덩이)";
	}
	else
	{
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 코인 가격 변동없음	(1Coin = " + std::to_string(CoinManager->Return_Coin_Pirce()) + "금덩이)";

	}

}

void DoBankWork::Exit(GoldCoinManager* CoinManager)
{
}

bool DoBankWork::OnMessage(GoldCoinManager* CoinManager, const Telegram& msg)
{
	return false;
}

//------------------------------------------------------------------------VisitBathroom

DrinkCoffe* DrinkCoffe::Instance()
{
	static DrinkCoffe instance;

	return &instance;
}


void DrinkCoffe::Enter(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ":  카페로 간다. 아아 주세요";
}


void DrinkCoffe::Execute(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 아! 시원하다!";

	CoinManager->GetFSM()->RevertToPreviousState();
}

void DrinkCoffe::Exit(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ":카페에서 나온다";
}


bool DrinkCoffe::OnMessage(GoldCoinManager* CoinManager, const Telegram& msg)
{
	return false;
}


//------------------------------------------------------------------------CookStew

SellCoin* SellCoin::Instance()
{
	static SellCoin instance;

	return &instance;
}


void SellCoin::Enter(GoldCoinManager* CoinManager)
{
	//if not already cooking put the stew in the oven
	if (!CoinManager->Cooking())
	{
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 현재 1골드 코인은 2금덩이입니다.";

		//send a delayed message myself so that I know when to take the stew
		//out of the oven
		Dispatch->DispatchMessage(1.5,                  //time delay
			CoinManager->ID(),           //sender ID
			CoinManager->ID(),           //receiver ID
			Msg_StewReady,        //msg
			NO_ADDITIONAL_INFO);

		CoinManager->SetCooking(true);
	}
}


void SellCoin::Execute(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 고객의 결정을 기다린다";
}

void SellCoin::Exit(GoldCoinManager* CoinManager)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": 스튜를 식탁에 둔다";
}


bool SellCoin::OnMessage(GoldCoinManager* CoinManager, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_StewReady:
	{
		cout << "\nMessage received by " << GetNameOfEntity(CoinManager->ID()) <<
			" at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": StewReady! Lets eat";

		//let hubby know the stew is ready
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
			CoinManager->ID(),
			ent_Miner_Bob,
			Msg_StewReady,
			NO_ADDITIONAL_INFO);

		CoinManager->SetCooking(false);

		CoinManager->GetFSM()->ChangeState(DoBankWork::Instance());
	}

	return true;

	}//end switch

	return false;
}