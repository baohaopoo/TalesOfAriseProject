#pragma once

#include "Base.h"

/*1. �� ���ӿ� ���� ��ü(�纻)���� �����ϰ� �����Ѵ�. */
/*2. ������ü���� �����Ѵ�. */
/*3. �纻��ü���� ������ ����Ѵ�.(Tick�Լ�ȣ��) */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	CGameObject* Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg);
	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);
	HRESULT Clear(_uint iLevelIndex);

private:
	/* ������ü���� �����Ѵ�. */
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;
		
	
private:
	/* �纻��ü���� ���̾�α���(�� �����)�Ͽ� �����Ѵ�.  */
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;

private:
	_uint			m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END