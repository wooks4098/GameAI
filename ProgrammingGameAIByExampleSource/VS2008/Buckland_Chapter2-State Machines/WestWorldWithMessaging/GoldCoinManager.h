#pragma once
#include <string>

//#include "BaseGameEntity.h"
//#include "Locations.h"
//#include "misc/ConsoleUtils.h"
//#include "GoldCoinManagerOwnedStates.h"
//#include "fsm/StateMachine.h"
//#include "misc/Utils.h"

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "GoldCoinManagerOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "Miner.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"


class GoldCoinManager : public BaseGameEntity
{
private:
	int Miner_coin;//광부가 들고 있는 코인 개수
	int Coin_Pirce;//현재 1코인을 살 수 있는 금덩이 개수

	//an instance of the state machine class
	StateMachine<GoldCoinManager>* m_pStateMachine;

	location_type   m_Location;

	//is she presently cooking?
	bool            m_bCooking;



public:

	GoldCoinManager(int id) :m_Location(shack),
		m_bCooking(false),
		BaseGameEntity(id)

	{
		Miner_coin = 0;
		Coin_Pirce = 3;


		//set up the state machine
		m_pStateMachine = new StateMachine<GoldCoinManager>(this);

		m_pStateMachine->SetCurrentState(DoBankWork::Instance());

		m_pStateMachine->SetGlobalState(GoldCoinManagerGlobalState::Instance());
	}

	~GoldCoinManager() { delete m_pStateMachine; }


	//this must be implemented
	void          Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);

	StateMachine<GoldCoinManager>* GetFSM()const { return m_pStateMachine; }

	//----------------------------------------------------accessors
	location_type Location()const { return m_Location; }
	void          ChangeLocation(location_type loc) { m_Location = loc; }

	bool          Cooking()const { return m_bCooking; }
	void          SetCooking(bool val) { m_bCooking = val; }
	
	int Return_Miner_coin(){ return Miner_coin;}
	void Set_Miner_Coin(int _coin) { Miner_coin += _coin; }

	int Return_Coin_Pirce(){ return Coin_Pirce; }
	void Set_Coin_Pirce(int _coin) { Coin_Pirce += _coin; }

};

