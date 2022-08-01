#pragma once
#include "VIBuffer.h"
#include "Model.h"
#include "HierarchyNode.h"

BEGIN(Engine)

class CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;
public:
	HRESULT NativeConstruct_Prototype(CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix, vector<CHierarchyNode*>	HierarchyNodes);
	HRESULT NativeConstruct(void* pArg);
public:
	_uint Get_NumMaterialIndex() const;
public:
	HRESULT Create_VertexBuffer_NonAnim(aiMesh* pAIMesh);
	HRESULT Create_VertexBuffer_Anim(aiMesh* pAIMesh, vector<CHierarchyNode*>	HierarchyNodes);
	HRESULT Create_SkinnedInfo(aiMesh* pAIMesh, VTXANIMMODEL* pVertices, vector<CHierarchyNode*>	HierarchyNodes);
	HRESULT Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);
private:
	CModel::TYPE m_eType = CModel::TYPE_END;
	_float4x4 m_PivotMatrix;
	_uint m_iMaterialIndex = 0;
private:
	aiMesh* m_pAIMesh = nullptr;
private:
	_uint m_iNumBones = 0;
	vector <CHierarchyNode*> m_MeshContainerNodes;
private:
	CHierarchyNode* m_pHierarchyNode = nullptr;
public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix, vector<CHierarchyNode*> HierarchyNodes);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END