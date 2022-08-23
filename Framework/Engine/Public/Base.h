#pragma once

#include "Engine_Defines.h"

/* 1. 모든 클래스들의 부모가된다. */
/* 2. 레퍼런스카운트에 대한 관리를 한다. */
/* 2_1. 레퍼런스카운트 : 참조갯수. 참조(객체메모리저장을 통해 접근가능하도록 만든 상태:포인터) */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* 레퍼런스카운트를 증가시킨다. */
	/* unsigned long  : 증가시키고난이후의 레퍼런스카운트를 반환*/
	unsigned long AddRef();

	/* 레퍼런스카운트를 감소시킨다. or 삭제한다.  */
	/* unsigned long  : 감소하기 이전에 레퍼런스 카운트의 값. */
	unsigned long Release();

private:
	/*참조갯수를 저장하고있는 변수이다. */
	unsigned long	m_dwRefCnt = 0;

public:
	virtual void Free() = 0;
};

END