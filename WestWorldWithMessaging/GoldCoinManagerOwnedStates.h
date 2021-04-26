#pragma once
#include "fsm/State.h"
#include <string>
class GoldCoinManager;

class GoldCoinManagerGlobalState : public State<GoldCoinManager>
{
private:

	GoldCoinManagerGlobalState() {}

	//copy ctor and assignment should be private
	GoldCoinManagerGlobalState(const GoldCoinManagerGlobalState&);
	GoldCoinManagerGlobalState& operator=(const GoldCoinManagerGlobalState&);

public:

	//this is a singleton
	static GoldCoinManagerGlobalState* Instance();

	virtual void Enter(GoldCoinManager* wife) {}

	virtual void Execute(GoldCoinManager* wife);

	virtual void Exit(GoldCoinManager* wife) {}

	virtual bool OnMessage(GoldCoinManager* wife, const Telegram& msg);
};




//���� ������ Ȯ��
class DoBankWork : public State< GoldCoinManager>
{
private:
	enum Coin_Fluctuation_rate
	{
		fall,
		up,
		keep
	};
	DoBankWork() {}

	//copy ctor and assignment should be private
	DoBankWork(const DoBankWork&);
	DoBankWork& operator=(const DoBankWork&);

public:

	//this is a singleton
	static DoBankWork* Instance();

	virtual void Enter(GoldCoinManager* wife);

	virtual void Execute(GoldCoinManager* wife);

	virtual void Exit(GoldCoinManager* wife);

	virtual bool OnMessage(GoldCoinManager* wife, const Telegram& msg);
};

//Ŀ�� ���ñ�
class DrinkCoffe : public State<GoldCoinManager>
{
private:

	DrinkCoffe() {}

	//copy ctor and assignment should be private
	DrinkCoffe(const DrinkCoffe&);
	DrinkCoffe& operator=(const DrinkCoffe&);

public:

	//this is a singleton
	static DrinkCoffe* Instance();

	virtual void Enter(GoldCoinManager* wife);

	virtual void Execute(GoldCoinManager* wife);

	virtual void Exit(GoldCoinManager* wife);

	virtual bool OnMessage(GoldCoinManager* wife, const Telegram& msg);

};


class SellCoin : public State<GoldCoinManager>
{
private:

	SellCoin() {}

	//copy ctor and assignment should be private
	SellCoin(const SellCoin&);
	SellCoin& operator=(const SellCoin&);

public:

	//this is a singleton
	static SellCoin* Instance();

	virtual void Enter(GoldCoinManager* wife);

	virtual void Execute(GoldCoinManager* wife);

	virtual void Exit(GoldCoinManager* wife);

	virtual bool OnMessage(GoldCoinManager* wife, const Telegram& msg);
};
