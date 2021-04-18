#include "GoldCoinManager.h"
bool GoldCoinManager::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void GoldCoinManager::Update()
{
	//set text color to green
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	m_pStateMachine->Update();
}