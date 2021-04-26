#pragma once
#include "fsm/State.h"
#include <string>

class Cat_Master;
class Cat_MasterGlobalState : public State<Cat_Master>
{
private:

	Cat_MasterGlobalState() {}

	//copy ctor and assignment should be private
	Cat_MasterGlobalState(const Cat_MasterGlobalState&);
	Cat_MasterGlobalState& operator=(const Cat_MasterGlobalState&);

public:

	//this is a singleton
	static Cat_MasterGlobalState* Instance();

	virtual void Enter(Cat_Master* catMaster) {}

	virtual void Execute(Cat_Master* catMaster);

	virtual void Exit(Cat_Master* catMaster) {}

	virtual bool OnMessage(Cat_Master* catMaster, const Telegram& msg);
};


#pragma region 일하기
class DoWork : public State< Cat_Master>
{
private:
	DoWork() {}

	//copy ctor and assignment should be private
	DoWork(const DoWork&);
	DoWork& operator=(const DoWork&);

public:

	//this is a singleton
	static DoWork* Instance();

	virtual void Enter(Cat_Master* catMaster);

	virtual void Execute(Cat_Master* catMaster);

	virtual void Exit(Cat_Master* catMaster);

	virtual bool OnMessage(Cat_Master* catMaster, const Telegram& msg);
};
#pragma endregion

#pragma region 밥주기;
class Feeding : public State< Cat_Master>
{
private:
	Feeding() {}

	//copy ctor and assignment should be private
	Feeding(const Feeding&);
	Feeding& operator=(const Feeding&);

public:

	//this is a singleton
	static Feeding* Instance();

	virtual void Enter(Cat_Master* catMaster);

	virtual void Execute(Cat_Master* catMaster);

	virtual void Exit(Cat_Master* catMaster);

	virtual bool OnMessage(Cat_Master* catMaster, const Telegram& msg);
};
#pragma endregion

#pragma region 청소하기
class Cleaning : public State< Cat_Master>
{
private:
	Cleaning() {}

	//copy ctor and assignment should be private
	Cleaning(const Cleaning&);
	Cleaning& operator=(const Cleaning&);

public:

	//this is a singleton
	static Cleaning* Instance();

	virtual void Enter(Cat_Master* catMaster);

	virtual void Execute(Cat_Master* catMaster);

	virtual void Exit(Cat_Master* catMaster);

	virtual bool OnMessage(Cat_Master* catMaster, const Telegram& msg);
};
#pragma endregion
