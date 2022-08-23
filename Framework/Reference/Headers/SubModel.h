#pragma once
#include "Component.h"

BEGIN(Engine)

class CAnimation;
class CTexture;
class CShader;
class CHierarchyNode;

class ENGINE_DLL CSubModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM,TYPE_EFFECT, TYPE_END };
public:
	CSubModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSubModel(const CSubModel& rhs);
	virtual ~CSubModel() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual HRESULT NativeConstruct(void* pArg) override;
	void Update(_double TimeDelta, _bool bCut);
	void Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex);
	HRESULT Render(class CShader* pShader, const char* pBoneMatricesName, _uint iMeshContainerIndex, _uint iPassIndex);
public:
	_uint Get_NumMeshContainer() const;
	CAnimation* Get_Animation(_uint iIndex);
public:
	void Set_AnimationIndex(_uint iAnimIndex);
public:
	HRESULT Bind_Material_OnShader(class CShader* pShader, aiTextureType eType, const char* pConstantName, _uint iMeshContainerIndex);
private:
	HRESULT Ready_MeshContainers();
	HRESULT Clone_MeshContainers();
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Animations();
	HRESULT Clone_Animations();	
	HRESULT Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent = nullptr, _uint iDepth = 0);
private:
	const aiScene* m_pScene = nullptr;
	Assimp::Importer m_Importer;
private:
	TYPE m_eType = TYPE_END;
	_float4x4 m_PivotMatrix;
private:
	_uint m_iNumMeshContainers = 0;
	vector<class CMeshContainer*> m_MeshContainers;
	typedef vector<class CMeshContainer*> MESHCONTAINERS;
private: 
	_uint m_iNumMaterials = 0;
	vector<MODELMATERIAL> m_Materials;
	typedef vector<MODELMATERIAL> MATERIALS;
private:
	_uint m_iNumAnimations = 0, m_iCurrentAnimIndex = 0;
	vector<CAnimation*> m_Animations;
	typedef vector<CAnimation*> ANIMATIONS;
private:
	_uint m_iNumNodes = 0;
	vector<CHierarchyNode*> m_HierarchyNodes;
	typedef vector<CHierarchyNode*> HIERARCHYNODES;
public:
	static CSubModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END