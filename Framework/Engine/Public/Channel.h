#pragma once
#include "Base.h"


BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;
public:
	HRESULT NativeConstruct_Prototype(aiNodeAnim* pAIChannel);
	HRESULT NativeConstruct_Prototype(CHANNELDESC* pData);
	HRESULT NativeConstruct();
	void Compute_TransformationMatrix(_double PlayTime);
	void Compute_MonsterTransformationMatrix(_double PlayTime);

public:
	_uint Get_CurrentKeyFrameIndex() { return m_iCurrentKeyFrame; }
	_uint Get_NumeKeyFrames() { return (_uint)m_KeyFrames.size(); }
	KEYFRAME* Get_KeyFrame(_uint iKeyFrame);

	class CHierarchyNode* Get_HierarchyNode();
public:
	void SetUp_HierarchyNodePtr(class CHierarchyNode* pNode);
	void Set_CurrentKeyFrame(_uint iKeyFrame);

public:
	void Set_PreMatrixZeroTrans() { XMStoreFloat4((_float4*)&m_PreMatrix.m[3][0], XMVectorSet(0.f, 0.f, 0.f, 1.f)); } //추가 정진구
	void Set_Start(_bool bStart) { m_bStart = bStart; } //추가 정진구

public:
	CHANNELDESC Get_Data() { return *m_pData; }

private:
	HRESULT Ready_KeyFrames(aiNodeAnim* pAIChannel);
	HRESULT Ready_KeyFrames(CHANNELDESC* pData);
private:
	char m_szName[MAX_PATH] = "";
private:
	_uint m_iNumKeyFrames = 0, m_iCurrentKeyFrame = 0;
	vector<KEYFRAME*> m_KeyFrames;
	typedef vector<KEYFRAME*> KEYFRAMES;
private:
	class CHierarchyNode* m_pHierarchyNode = nullptr;
private:
	_float4x4 m_TransformationMatrix;
	_bool m_isCloned = false;
private:
	_float4x4 m_PreMatrix;
	_bool m_bStart = false;
private:
	CHANNELDESC* m_pData = nullptr;

public:
	static CChannel* Create(aiNodeAnim* pAIChannel);
	static CChannel* Create(CHANNELDESC* pData);
	CChannel* Clone();
	virtual void Free() override;
};

END