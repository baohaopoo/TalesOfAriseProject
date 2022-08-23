#pragma once
#include "Base.h"

BEGIN(Engine)

class CConsole_Debug final : public CBase
{
	DECLARE_SINGLETON(CConsole_Debug)
public:
	CConsole_Debug();
	virtual ~CConsole_Debug() = default;
public:
	HRESULT NativeConstruct();
public:
	CRITICAL_SECTION Get_CriticalSection();
private:
	HANDLE m_hThread = 0;
	CRITICAL_SECTION m_CriticalSection;
private:
	_bool m_bInit = false;
public:
	virtual void Free() override;
};

END