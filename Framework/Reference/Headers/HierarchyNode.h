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
	HRESULT NativeConstruct(HIERARCHYDESC* pData);
	void Update_CombinedTransformationMatrix();
	HIERARCHYDESC Get_Data();
public:
	const char* Get_Name();
	CHierarchyNode* Get_Parent();
	_uint Get_Depth() const;
	_matrix Get_OffsetMatrix() const;
	_matrix Get_CombinedMatrix() const;
	_matrix Get_MoveMatrix();
	_int Get_Index() { return m_Index; }
	_float3 Get_MoveVector()
	{
		return m_vMoveVector;
	}
	_int Get_ParentIndex() { return m_ParentIndex; }
	CHierarchyNode* Get_HierarchyParent() { return m_pParent; }
	_float4x4* Get_CombinedFloat4x4Ptr() {
		return &m_CombinedTransformationMatrix;
	}


	void Set_ParentIndex(_int flag) { m_ParentIndex = flag; }
	void Set_MoveVector(_fvector vMoveVector) //추가정진구
	{
		XMStoreFloat3(&m_vMoveVector, vMoveVector);
	}
	void Set_Index(_int flag) { m_Index = flag; }
	void Set_HierarchyParent(CHierarchyNode* flag) { m_pParent = flag; }

public:
	void Set_TransformationMatrix(_fmatrix TransformationMatrix);
	void Set_OffsetMatrix(_fmatrix OffsetMatrix);
	void Set_OriginTransMatrix(_fmatrix OriginTransMatrix);
	void Set_CurrentFrameMatrix(_fmatrix CurrentFrameMatrix);
private:
	char m_szName[MAX_PATH] = "";
	_uint m_iDepth = 0;
	_int m_Index = 0;
	_int m_ParentIndex = 0;
	CHierarchyNode*	m_pParent = nullptr;
	_float4x4 m_OffsetMatrix;
	_float4x4 m_TransformationMatrix;
	_float4x4 m_CombinedTransformationMatrix;
private:
	HIERARCHYDESC* m_pData = nullptr;
private:
	_float4x4 m_OriginTransMatrix;
	_float4x4 m_CurrentFrameMatrix;
	_float3 m_vMoveVector;
	_float4x4 m_TransMatrix;
public:
	static CHierarchyNode* Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	static CHierarchyNode* Create(HIERARCHYDESC* pData);
	virtual void Free() override;
};

END