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
			"�մ� ���� ��ǰ�� �ֽ��ϴ�. ���� ���簡 �ȴٱ���!!";
		Dispatch->DispatchMessage(1.5,                  //time delay
			CoinManager->ID(),           //sender ID
			ent_Miner_Bob,           //receiver ID
			Msg_Deal,        //msg  Msg_Deal
			CoinManager->Return_Coin_Pirce(),
			NO_ADDITIONAL_INFO);
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
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": ���� �������� Ȯ���� �ð��̴�";
}


void DoBankWork::Execute(GoldCoinManager* CoinManager)
{
	int random = RandInt(0, 9);

	if (random >= 7) //���� ���
	{
		CoinManager->Set_Coin_Pirce(+1);
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": ���� ���� ���	(1Coin = " + std::to_string(CoinManager->Return_Coin_Pirce()) + "�ݵ���)";
	}
	else if (random >= 3 && random <= 6 && CoinManager->Return_Coin_Pirce() >= 2) //���� �϶�
	{
		CoinManager->Set_Coin_Pirce(-1);
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": ���� ���� �϶�	(1Coin = " + std::to_string(CoinManager->Return_Coin_Pirce()) + "�ݵ���)";
	}
	else
	{
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": ���� ���� ��������	(1Coin = " + std::to_string(CoinManager->Return_Coin_Pirce()) + "�ݵ���)";

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
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ":  ī��� ����. �ƾ� �ּ���";
}


void DrinkCoffe::Execute(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": ��! �ÿ��ϴ�!";

	CoinManager->GetFSM()->RevertToPreviousState();
}

void DrinkCoffe::Exit(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ":ī�信�� ���´�";
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
		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << 
			": ���� 1��� ������ "+ std::to_string(CoinManager->Return_Coin_Pirce())+ "�ݵ��� �Դϴ�. ";

		//send a delayed message myself so that I know when to take the stew
		

	}
}


void SellCoin::Execute(GoldCoinManager* CoinManager)
{
	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": ���� ������ ��ٸ���";
}

void SellCoin::Exit(GoldCoinManager* CoinManager)
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": �ŷ� �����մϴ�";
}


bool SellCoin::OnMessage(GoldCoinManager* CoinManager, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_Deal_Success:
	{
		cout << "\nMessage received by " << GetNameOfEntity(CoinManager->ID()) <<
			" at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": �ŷ� �����մϴ�";



		CoinManager->GetFSM()->ChangeState(DoBankWork::Instance());
	}
	case Msg_Deal_Fail:
	{
		cout << "\nMessage received by " << GetNameOfEntity(CoinManager->ID()) <<
			" at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(CoinManager->ID()) << ": �ð� ���ּż� �����մϴ�";



		CoinManager->GetFSM()->ChangeState(DoBankWork::Instance());

	}

	return true;

	}//end switch

	return false;
}