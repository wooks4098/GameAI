#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

#pragma region �ݱ�
//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance;

	return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�ݱ����� ���Ѵ�";

		pMiner->ChangeLocation(goldmine);
	}
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�ݵ��̸� ������";

	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		//�������� ���� ���ٴ� �޽��� ����
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Gold_Coin_Manager,            //ID of recipient
			Msg_GoBank,   //the message
			NO_ADDITIONAL_INFO);
		
		pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
		
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "�ָӴϿ� �ݵ��̸� ���� ä��� �ݱ��� ������";
	
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	/*case Msg_Deal:

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->SetCoin_Pirce(msg.Coin);
		pMiner->GetFSM()->ChangeState(Deal_Coin::Instance());

		return true;
*/
	}//end switch
	return false;
}

#pragma endregion

#pragma region ����
//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance;

	return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�������� ����.";

		pMiner->ChangeLocation(bank);

		
	}
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
	//deposit the gold
	
	
	//wealthy enough to have a well earned rest?
	if (end_deal)
	{
		pMiner->AddToWealth(pMiner->GoldCarried());

		pMiner->SetGoldCarried(0);
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "���� �����Ѵ�. ���� ���� ���෮: " << pMiner->Wealth();

		if (pMiner->Wealth() >= ComfortLevel)
		{
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
				<< "����! ���� ����� ���ڰ� �Ǿ���. ���� �Ϳ��� �Ƴ����Է� ���ư���.";
			end_deal = false;
			pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
		}

		//otherwise get more gold
		else
		{
			end_deal = false;
			pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
		}
	}

}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	if (!end_deal)
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ������";
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{

	case Msg_Deal:
		end_deal = true;
		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->SetCoin_Pirce(msg.Coin);
		pMiner->GetFSM()->ChangeState(Deal_Coin::Instance());

		return true;

	}//end switch
	return false;
}
#pragma endregion

#pragma region ���ڷ� ������
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
	static GoHomeAndSleepTilRested instance;

	return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
	if (pMiner->Location() != shack)
	{
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ �ɾ��";

		pMiner->ChangeLocation(shack);

		//let the wife know I'm home
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiHoneyImHome,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued())
	{
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "��� �Ƿΰ� �������. ���� ã���� �� �ð��̴�";

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
	}
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_StewReady:
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": Okay Hun, ahm a comin'!";

		pMiner->GetFSM()->ChangeState(EatStew::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}
#pragma endregion

#pragma region �����÷� ����
//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;

	return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ������, �������� �ɾ��";
	}
}

void QuenchThirst::Execute(Miner* pMiner)
{
	pMiner->BuyAndDrinkAWhiskey();
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���� ���̾�.";

	pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
}


void QuenchThirst::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ������, ��е� ����";
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
#pragma endregion


#pragma region ��Ʃ�Ա�
//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
	static EatStew instance;

	return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ���� ���� Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���� ����!";

	pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ����. �ϴ� ���� �ٽ� �ϴ� ���� ���ڱ�.";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
#pragma endregion

#pragma region ���� �ŷ�

//------------------------------------------------------------------------Deal_Coin


Deal_Coin* Deal_Coin::Instance()
{
	static Deal_Coin instance;

	return &instance;
}


void Deal_Coin::Enter(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���簡 �ȴٰ��??";
}

void Deal_Coin::Execute(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	/*cout << "\n" << GetNameOfEntity(pMiner->ID())
		<< ": �����̶� ����...";*/

	if (pMiner->Wealth() >= pMiner->GetCoin_Pirce())
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": �ŷ��սô� ";
		//�������� �ŷ��Ѵٴ� �޽��� ����
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Gold_Coin_Manager,            //ID of recipient
			Msg_Deal_Success,   //the message
			NO_ADDITIONAL_INFO);
		pMiner->SetWealth(-pMiner->GetCoin_Pirce());
	}
	else
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": ���� �����ϱ��� �����մϴ� ";
		//�������� �ŷ� ���Ѵٴ� �޽��� ����
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Gold_Coin_Manager,            //ID of recipient
			Msg_Deal_Fail,   //the message
			NO_ADDITIONAL_INFO);

	}

	pMiner->GetFSM()->RevertToPreviousState();
}

void Deal_Coin::Exit(Miner* pMiner)
{
}


bool Deal_Coin::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_Deal:

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->SetCoin_Pirce(msg.Coin);
		pMiner->GetFSM()->ChangeState(Deal_Coin::Instance());

		return true;

	}//end switch

	return false;
}
#pragma endregion


