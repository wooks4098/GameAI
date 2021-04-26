#pragma once
#include "fsm/State.h"
#include <string>

class Cat;
class CatGlobalState : public State<Cat>
{
private:

	CatGlobalState() {}

	//copy ctor and assignment should be private
	CatGlobalState(const CatGlobalState&);
	CatGlobalState& operator=(const CatGlobalState&);

public:

	//this is a singleton
	static CatGlobalState* Instance();

	virtual void Enter(Cat* cat) {}

	virtual void Execute(Cat* cat);

	virtual void Exit(Cat* cat) {}

	virtual bool OnMessage(Cat* cat, const Telegram& msg);
};


#pragma region ½Ä»§ÀÚ¼¼
class Bread_posture : public State< Cat>
{
private:
	Bread_posture() {}

	//copy ctor and assignment should be private
	Bread_posture(const Bread_posture&);
	Bread_posture& operator=(const Bread_posture&);

public:

	//this is a singleton
	static Bread_posture* Instance();

	virtual void Enter(Cat* cat);

	virtual void Execute(Cat* cat);

	virtual void Exit(Cat* cat);

	virtual bool OnMessage(Cat* cat, const Telegram& msg);
};
#pragma endregion

#pragma region ¹ä¸Ô±â
class Eat : public State< Cat>
{
private:
	Eat() {}

	//copy ctor and assignment should be private
	Eat(const Eat&);
	Eat& operator=(const Eat&);

public:

	//this is a singleton
	static Eat* Instance();

	virtual void Enter(Cat* cat);

	virtual void Execute(Cat* cat);

	virtual void Exit(Cat* cat);

	virtual bool OnMessage(Cat* cat, const Telegram& msg);
};

#pragma endregion

#pragma region ¹°°Ç ±ú±â

class Break : public State< Cat>
{
private:
	Break() {}

	//copy ctor and assignment should be private
	Break(const Break&);
	Break& operator=(const Break&);

public:

	//this is a singleton
	static Break* Instance();

	virtual void Enter(Cat* cat);

	virtual void Execute(Cat* cat);

	virtual void Exit(Cat* cat);

	virtual bool OnMessage(Cat* cat, const Telegram& msg);
};

#pragma endregion