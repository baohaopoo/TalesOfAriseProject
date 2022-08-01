#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::NativeConstruct(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	strcpy_s(m_szName, pAINode->mName.data);
	m_pParent = pParent;
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	m_iDepth = iDepth;

	XMStoreFloat4x4(&m_CurrentFrameMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());
	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	if (nullptr != m_pParent)
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
		XMStoreFloat4x4(&m_OriginTransMatrix, XMLoadFloat4x4(&m_OriginTransMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
		XMStoreFloat4x4(&m_CurrentFrameMatrix, XMLoadFloat4x4(&m_CurrentFrameMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix));
	}
}

const char* CHierarchyNode::Get_Name()
{
	return m_szName;
}

CHierarchyNode* CHierarchyNode::Get_Parent()
{
	return m_pParent;
}

_uint CHierarchyNode::Get_Depth() const
{
	return m_iDepth;
}

_matrix CHierarchyNode::Get_OffsetMatrix() const
{
	return XMLoadFloat4x4(&m_OffsetMatrix);
}

_matrix CHierarchyNode::Get_CombinedMatrix() const
{
	return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
}

_matrix CHierarchyNode::Get_MoveMatrix()
{
	_matrix MoveMatrix = XMLoadFloat4x4(&m_OriginTransMatrix);
	MoveMatrix.r[3] = XMLoadFloat4x4(&m_OriginTransMatrix).r[3] - XMLoadFloat4x4(&m_CurrentFrameMatrix).r[3];
	return MoveMatrix;
}

void CHierarchyNode::Set_TransformationMatrix(_fmatrix TransformationMatrix)
{
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
}

void CHierarchyNode::Set_OffsetMatrix(_fmatrix OffsetMatrix)
{
	XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
}

void CHierarchyNode::Set_OriginTransMatrix(_fmatrix OriginTransMatrix)
{
	XMStoreFloat4x4(&m_OriginTransMatrix, OriginTransMatrix);
}

void CHierarchyNode::Set_CurrentFrameMatrix(_fmatrix CurrentFrameMatrix)
{
	XMStoreFloat4x4(&m_CurrentFrameMatrix, CurrentFrameMatrix);
}

CHierarchyNode* CHierarchyNode::Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode*	pInstance = new CHierarchyNode();
	if (FAILED(pInstance->NativeConstruct(pAINode, pParent, iDepth)))
	{
		MSG_BOX(L"CHierarchyNode -> Create -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierarchyNode::Free()
{
}