#pragma once
#include "Base.h"


BEGIN(Engine)

class CChannel;
class CHierarchyNode;

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;
public:
	HRESULT NativeConstruct_Prototype(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT NativeConstruct_Prototype(ANIMATIONDESC* pData, vector<class CHierarchyNode*> Nodes);
	HRESULT NativeConstruct(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT NativeConstruct(ANIMATIONDESC* pData, vector<class CHierarchyNode*> Nodes);
	void Update(_double TimeDelta, _bool bRoof);
	void MonsterUpdate(_double TimeDelta, _bool bRoof);
	void Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex, _uint* pCurrentAnimIndexModel, vector<CChannel*> vChannel);
	void MonsterUpdate_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex, _uint* pCurrentAnimIndexModel, vector<CChannel*> vChannel);

public:
	vector<CChannel*> Get_Channels();
	_bool Get_Finish();
	_double Get_TimeAcc();
	void Reset_Animation(); //추가정진구
	void Reset_MonsterAnimation();
	CChannel * Get_MainChannel();
	CChannel * Get_PelvisChannel();
	CChannel * Get_CenterChannel();
	_float3 Get_MoveVector(); //추가정진구
	_matrix Get_MoveMatrix();
	ANIMATIONDESC Get_Data() { return *m_pData; }
public:

	void Set_ZeroFrame();
private:
	char m_szName[MAX_PATH] = "";
	_double m_Duration = 0.0, m_TickPerSecond = 0.0, m_TimeAcc = 0.0, m_TimeAccNext = 0.0;
	_bool m_isFinished = false;
private:
	_uint m_iNumChannels = 0;
	vector<CChannel*> m_Channels;
	typedef vector<CChannel*> CHANNELS;
	CChannel* m_pMainChannel = nullptr;
	CChannel* m_pPelvisChannel = nullptr;
	CChannel* m_pCenterChannel = nullptr;
private:
	ANIMATIONDESC* m_pData = nullptr;
private:
	HRESULT Ready_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT Ready_Channels(ANIMATIONDESC* pData, vector<CHierarchyNode*> Nodes);
	HRESULT Clone_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT Clone_Channels(ANIMATIONDESC* pData, vector<CHierarchyNode*> Nodes);

public:
	static CAnimation* Create(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	static CAnimation* Create(ANIMATIONDESC* pData, vector<CHierarchyNode*> Nodes);
	CAnimation* Clone(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	CAnimation* Clone(ANIMATIONDESC* pData, vector<CHierarchyNode*> Nodes);
	virtual void Free() override;
};

END
