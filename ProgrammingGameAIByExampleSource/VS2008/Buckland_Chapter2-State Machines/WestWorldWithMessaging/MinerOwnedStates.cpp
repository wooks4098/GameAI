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

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�ݵ��̸� ������";

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "�ָӴϿ� �ݵ��̸� ���� ä��� �ݱ��� ������";
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

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
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�������� ����.";

    pMiner->ChangeLocation(bank);

	//�������� ���� ���ٴ� �޽��� ����
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_Gold_Coin_Manager,            //ID of recipient
		Msg_GoBank,   //the message
		NO_ADDITIONAL_INFO);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "���� �����Ѵ�. ���� ���� ���෮: "<< pMiner->Wealth();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
         << "����! ���� ����� ���ڰ� �Ǿ���. ���� �Ϳ��� �Ƴ����Է� ���ư���.";
      
    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());      
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ������";
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
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
   SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_StewReady:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID()) 
          << ": Okay Hun, ahm a comin'!";

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
      
     return true;

   }//end switch

   return false; //send message to global message handler
}

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

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ������, �������� �ɾ��";
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���� ���̾�.";

  pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());  
}


void QuenchThirst::Exit(Miner* pMiner)
{ 
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ������, ��е� ����";
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ���� ���� Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���� ����!";

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{ 
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ����. �ϴ� ���� �ٽ� �ϴ� ���� ���ڱ�.";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------Deal_Coin


Deal_Coin* Deal_Coin::Instance()
{
	static Deal_Coin instance;

	return &instance;
}


void Deal_Coin::Enter(Miner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���簡 �ȴٰ���??";
	isBuy = false;
}

void Deal_Coin::Execute(Miner* pMiner)
{
	int random = RandInt(0, 1);

	if (random == 0)
	{
		isBuy = true;
	}
	else
	{
		isBuy = false;
	}


	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ���� ����!"; 

	pMiner->GetFSM()->RevertToPreviousState();
}

void Deal_Coin::Exit(Miner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "������ ����. �ϴ� ���� �ٽ� �ϴ� ���� ���ڱ�.";
}


bool Deal_Coin::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}