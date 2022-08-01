#pragma once
#include "Base.h"
#include "HierarchyNode.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	CChannel(const CChannel& rhs);
	virtual ~CChannel() = default;
public:
	HRESULT NativeConstruct_Prototype(aiNodeAnim* pAIChannel);
	HRESULT NativeConstruct();
	void Compute_TransformationMatrix(_double PlayTime);
public:
	_uint Get_CurrentKeyFrameIndex();
	KEYFRAME* Get_KeyFrame(_uint iKeyFrame);
	CHierarchyNode* Get_HierarchyNode();
public:
	void SetUp_HierarchyNodePtr(class CHierarchyNode* pNode);
	void Set_CurrentKeyFrame(_uint iKeyFrame);
private:
	HRESULT Ready_KeyFrames(aiNodeAnim* pAIChannel);
private:
	char m_szName[MAX_PATH] = "";
private:
	_uint m_iNumKeyFrames = 0, m_iCurrentKeyFrame = 0;
	vector<KEYFRAME*> m_KeyFrames;
	typedef vector<KEYFRAME*> KEYFRAMES;
private:
	CHierarchyNode* m_pHierarchyNode = nullptr;
private:
	_float4x4 m_TransformationMatrix;
	_bool m_isCloned = false;
private:
	_float4x4 m_PreMatrix;
public:
	static CChannel* Create(aiNodeAnim* pAIChannel);
	CChannel* Clone();
	virtual void Free() override;
};

END