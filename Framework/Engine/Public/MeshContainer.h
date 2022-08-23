#pragma once
#include "VIBuffer.h"
#include "Model.h"




BEGIN(Engine)
class CHierarchyNode;


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

public: //dat 파일로드 전용
	HRESULT NativeConstruct_Prototype(MESHCONTAINERDESC* pData, vector<CHierarchyNode*>	HierarchyNodes);
	HRESULT Data_VtxBuff_NonAnim(MESHCONTAINERDESC* pData);
	HRESULT Data_VtxBuff_Anim(MESHCONTAINERDESC* pData, vector<CHierarchyNode*>	HierarchyNodes);
	HRESULT Data_SkinnedInfo(MESHCONTAINERDESC* pData, VTXANIMMODEL* pVertices, vector<CHierarchyNode*>	HierarchyNodes);
	HRESULT Save_Data();
	MESHCONTAINERDESC Get_Data() { return *m_MeshContainerDesc; }

public:
	VTXNONANIMMODEL* Get_NonAnimVertices(void) { return m_pNonAnimVertices; }
	FACELISTINDICES32* Get_Indices(void) { return m_pIndices; }
	_uint Get_PrimitiveCnt(void) { return m_iNumPrimitive; }

private:
	VTXNONANIMMODEL*			m_pNonAnimVertices = nullptr;
	VTXANIMMODEL*				m_pAnimVertices = nullptr;
	FACELISTINDICES32*			m_pIndices = nullptr;

private:
	CModel::TYPE m_eType = CModel::TYPE_END;
	_float4x4 m_PivotMatrix;
	_uint m_iMaterialIndex = 0;
	MESHCONTAINERDESC*			m_MeshContainerDesc = nullptr;
	_bool						m_isBinary = false;

private:
	aiMesh* m_pAIMesh = nullptr;

private:
	_uint m_iNumBones = 0;
	vector <CHierarchyNode*> m_MeshContainerNodes;

private:
	CHierarchyNode* m_pHierarchyNode = nullptr;

public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CModel::TYPE eType, aiMesh* pAIMesh, _float4x4 PivotMatrix, vector<CHierarchyNode*> HierarchyNodes);
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, MESHCONTAINERDESC* pData, vector<class CHierarchyNode*> HierarchyNodes);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END