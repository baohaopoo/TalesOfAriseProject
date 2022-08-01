#pragma once

// 1.프레임관리할때 쓰기 좋다.
// 2.객체의 움직임을 제어할때 사용하기 좋다.
#include "Base.h"


// 1초에 업데이트가 100번호출된다.
// m_dwTimeAcc는 1초에 1만큼 증가하는 변수.
// m_dwTimeAcc 한프레임당 얼마씩 증가할까? 0.01씩 증가하는 변수.

// Player.x + 5.0f * 0.01(초);

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	HRESULT Ready_Timer();
	_double Compute_TimeDelta();
private:
	_double	m_TimeDelta = 0.0;
private:
	LARGE_INTEGER	m_FrameTime;
	LARGE_INTEGER	m_FixTime;
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_CpuTick;
public:
	static CTimer* Create();
	virtual void Free();
};

END