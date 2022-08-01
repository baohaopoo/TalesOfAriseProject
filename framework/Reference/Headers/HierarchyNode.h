#pragma once
#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;
public:
	HRESULT NativeConstruct(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	void Update_CombinedTransformationMatrix();
public:
	const char* Get_Name();
	CHierarchyNode* Get_Parent();
	_uint Get_Depth() const;
	_matrix Get_OffsetMatrix() const;
	_matrix Get_CombinedMatrix() const;
	_matrix Get_MoveMatrix();

	_float4x4* Get_CombinedFloat4x4Ptr() {
		return &m_CombinedTransformationMatrix;
	}
public:
	void Set_TransformationMatrix(_fmatrix TransformationMatrix);
	void Set_OffsetMatrix(_fmatrix OffsetMatrix);
	void Set_OriginTransMatrix(_fmatrix OriginTransMatrix);
	void Set_CurrentFrameMatrix(_fmatrix CurrentFrameMatrix);
private:
	char m_szName[MAX_PATH] = "";
	_uint m_iDepth = 0;
	CHierarchyNode*	m_pParent = nullptr;
	_float4x4 m_OffsetMatrix;
	_float4x4 m_TransformationMatrix;
	_float4x4 m_CombinedTransformationMatrix;
private:
	_float4x4 m_OriginTransMatrix;
	_float4x4 m_CurrentFrameMatrix;
public:
	static CHierarchyNode* Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	virtual void Free() override;
};

END