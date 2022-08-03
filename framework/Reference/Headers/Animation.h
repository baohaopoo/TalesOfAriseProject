#pragma once
#include "Base.h"
#include "Channel.h"
#include "HierarchyNode.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;
public:
	HRESULT NativeConstruct_Prototype(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT NativeConstruct(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	void Update(_double TimeDelta, _bool bRoof);
	void Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex, _uint* pCurrentAnimIndexModel, vector<CChannel*> vChannel);

public:
	vector<CChannel*> Get_Channels();
	_bool Get_Finish();
	_double Get_TimeAcc();
	_matrix Get_MoveMatrix();
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
private:
	HRESULT Ready_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	HRESULT Clone_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
public:
	static CAnimation* Create(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	CAnimation* Clone(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes);
	virtual void Free() override;


	// binary
public:
	HRESULT Save_AnimationInfo(HANDLE& hFile);
	static CAnimation* Create(HANDLE& hFile, vector<CHierarchyNode*> Nodes);
	HRESULT NativeConstruct_Prototype(HANDLE& hFile, vector<class CHierarchyNode*> Nodes);

public:
	CAnimation* Clone(vector<CHierarchyNode*> Nodes);
	HRESULT NativeConstruct(vector<class CHierarchyNode*> Nodes);
	HRESULT Clone_Dat_Channels(vector<CHierarchyNode*> Nodes);

public:
	_uint Get_KeyFrame_RootChannel(void) { return m_pChannel_Root->Get_CurrentKeyFrameIndex(); }

private:
	class CChannel*						m_pChannel_Root = nullptr;
};

END
