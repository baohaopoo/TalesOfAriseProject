#pragma once

#include "Base.h"

/* 현재 화면에 보여줘야하늕 레벨의 주소를 가지고 있는다. */
/* 현재 레벨의 틱 무한호출. */
/* 현재 레벨의 렌더 무한호출. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pLevel);
	void Tick(_double TimeDelta);
	HRESULT Render();

private:
	_uint					m_iLevelIndex = 0;
	class CLevel*			m_pCurrentLevel = nullptr;

public:
	virtual void Free() override;
};

END