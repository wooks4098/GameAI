#pragma once
#include <string>


#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "Cat_MasterOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "Miner.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"

class Cat_Master : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Cat_Master>* m_pStateMachine;


public:

	Cat_Master(int id) :
		BaseGameEntity(id)
	{
		//set up the state machine
		m_pStateMachine = new StateMachine<Cat_Master>(this);
		m_pStateMachine->SetCurrentState(DoWork::Instance());
		m_pStateMachine->SetGlobalState(Cat_MasterGlobalState::Instance());
	}

	~Cat_Master() { delete m_pStateMachine; }

	//this must be implemented
	void          Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);

	StateMachine<Cat_Master>* GetFSM()const { return m_pStateMachine; }
};

