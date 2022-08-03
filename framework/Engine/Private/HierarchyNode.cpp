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

HRESULT CHierarchyNode::SaveDatInfo(HANDLE & hFile)
{
	if (nullptr == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;
	DWORD		dwStrByte = 0;

	// 이름(m_szName) 저장
	// 문자열의 사이즈 구하기
	dwStrByte = strlen(m_szName) + 1;	// nullptr 을 위해 1개 추가

										// 해당 문자열의 사이즈 저장
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

	// 문자열의 메모리를 문자열의 사이즈만큼 저장
	WriteFile(hFile, m_szName, dwStrByte, &dwByte, nullptr);



	// 깊이 정보 저장
	WriteFile(hFile, &m_iDepth, sizeof(_uint), &dwByte, nullptr);

	// 부모 노드 정보 저장
	WriteFile(hFile, &m_iHierarchyNumber, sizeof(_uint), &dwByte, nullptr);

	// 행렬 정보 저장
	WriteFile(hFile, &m_TransformationMatrix, sizeof(_float4x4), &dwByte, nullptr);

	return S_OK;
}

CHierarchyNode * CHierarchyNode::Create(const char * szName, _matrix TransformationMatrixTP, _uint iDepth)
{
	CHierarchyNode*	pInstance = new CHierarchyNode();

	if (FAILED(pInstance->NativeConstruct(szName, TransformationMatrixTP, iDepth)))
	{
		MSG_BOX(TEXT("Failed to Created CHierarchyNode"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHierarchyNode * CHierarchyNode::Clone(void * pArg)
{
	CHierarchyNode*	pInstance = new CHierarchyNode(*this);

	return pInstance;
}

HRESULT CHierarchyNode::NativeConstruct(const char * szName, _matrix TransformationMatrixTP, _uint iDepth)
{
	// 이름 저장
	strcpy_s(m_szName, szName);

	// 이동행렬의 역행렬 값 저장
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrixTP);

	// 깊이 값 저장
	m_iDepth = iDepth;

	// 부모 노드의 정보는 외부에서 따로 처리한다.
	return S_OK;
}
