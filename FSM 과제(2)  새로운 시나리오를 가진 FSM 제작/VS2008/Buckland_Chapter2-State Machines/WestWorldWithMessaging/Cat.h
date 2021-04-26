#pragma once
#include <string>
#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "CatOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "Miner.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"
class Cat : public BaseGameEntity
{

private:

	//an instance of the state machine class
	StateMachine<Cat>* m_pStateMachine;

	//location_type   m_Location;

public:

	Cat(int id) :
		BaseGameEntity(id)
	{
		//set up the state machine
		m_pStateMachine = new StateMachine<Cat>(this);
		m_pStateMachine->SetCurrentState(DoWork::Instance());
		m_pStateMachine->SetGlobalState(Cat_MasterGlobalState::Instance());
	}

	~Cat() { delete m_pStateMachine; }

	//this must be implemented
	void          Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);

	StateMachine<Cat>* GetFSM()const { return m_pStateMachine; }

};

