#pragma once

#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;
public:
	HRESULT Add_Timers(const _tchar* pTimerTag);
	_double Compute_TimeDelta(const _tchar* pTimerTag);
private:
	map<const _tchar*, CTimer*>			m_Timers;
	typedef map<const _tchar*, CTimer*>	TIMERS;
private:
	CTimer* Find_Timers(const _tchar* pTimerTag);
public:
	virtual void Free();
};

END