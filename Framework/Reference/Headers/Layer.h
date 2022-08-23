#pragma once

#include "Base.h"


BEGIN(Engine)

class ENGINE_DLL CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex = 0);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);
	list<class CGameObject*>* Get_List();
private:
	list<class CGameObject*>			m_ObjectList;
	typedef list<class CGameObject*>	OBJECTLIST;
public:
	CGameObject * Get_GameObject(_uint iIndex);
	list<class CGameObject*>& Get_ObjectList() { return m_ObjectList; }
public:
	static CLayer* Create();
	virtual void Free() override;
};

END