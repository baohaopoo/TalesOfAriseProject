#include "..\Public\Channel.h"

#include "HierarchyNode.h"
CChannel::CChannel()
	:m_isCloned(false)
{
}

CChannel::CChannel(const CChannel& rhs)
	: m_KeyFrames(rhs.m_KeyFrames)
	, m_iNumKeyFrames(rhs.m_iNumKeyFrames)
	, m_iCurrentKeyFrame(rhs.m_iCurrentKeyFrame)
	, m_TransformationMatrix(rhs.m_TransformationMatrix)
	, m_isCloned(true)
{
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CChannel::NativeConstruct_Prototype(aiNodeAnim* pAIChannel)
{
	m_pData = new CHANNELDESC;

	strcpy_s(m_szName, pAIChannel->mNodeName.data);
	strcpy_s(m_pData->NodeName, pAIChannel->mNodeName.data);

	if (FAILED(Ready_KeyFrames(pAIChannel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChannel::NativeConstruct_Prototype(CHANNELDESC * pData)
{
	strcpy_s(m_szName, pData->NodeName);

	if (FAILED(Ready_KeyFrames(pData)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChannel::NativeConstruct()
{
	return S_OK;
}

void CChannel::Compute_TransformationMatrix(_double PlayTime)
{
	while (m_iCurrentKeyFrame + 1 < m_KeyFrames.size())
	{
		if (PlayTime > m_KeyFrames[m_iCurrentKeyFrame + 1]->Time)
		{
			++m_iCurrentKeyFrame;
		}
		else
		{
			break;
		}
	}
	KEYFRAME* pLastKeyFrame = m_KeyFrames.back();

	_vector vScale, vRotation, vTranslation;
	_matrix CurrentMatrix;
	if (PlayTime > pLastKeyFrame->Time)
	{
		vScale = XMLoadFloat3(&pLastKeyFrame->vScale);
		vRotation = XMLoadFloat4(&pLastKeyFrame->vRotation);
		vTranslation = XMLoadFloat3(&pLastKeyFrame->vTranslation);
		vTranslation = XMVectorSetW(vTranslation, 1.f);

		CurrentMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	}
	else
	{
		_double Ratio = (PlayTime - m_KeyFrames[m_iCurrentKeyFrame]->Time) / (m_KeyFrames[m_iCurrentKeyFrame + 1]->Time - m_KeyFrames[m_iCurrentKeyFrame]->Time);

		_vector vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame]->vScale);
		_vector vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame]->vRotation);
		_vector vSourTranslation = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame]->vTranslation);

		CurrentMatrix = XMMatrixAffineTransformation(vSourScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vSourRotation, vSourTranslation); //현재 키프레임 

		_vector vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vScale);
		_vector vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vRotation);
		_vector vDestTranslation = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vTranslation);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, (_float)Ratio);
	}
	_matrix Matrix;

	if (nullptr != m_pHierarchyNode)
	{
		Matrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);  //현재 내 로컬상의 최종 매트릭스 이동값 안죽인것

		if (!strcmp("TransN", m_pHierarchyNode->Get_Name()))
		{
			if (!m_bStart)
			{
				XMStoreFloat4x4(&m_PreMatrix, Matrix);
				m_bStart = true;
			}

			//if (!XMVector3Equal(XMLoadFloat4x4(&m_PreMatrix).r[3], Matrix.r[3]))
			//{
			//	XMStoreFloat4x4(&m_PreMatrix, Matrix);
			//}

			//m_pHierarchyNode->Set_CurrentFrameMatrix(CurrentMatrix);// 전애니메이션 틱의 Matrix 를 저장해서 쓰자.  //지금은 0번쨰 매트릭스를 던
			//m_pHierarchyNode->Set_OriginTransMatrix(Matrix);        // 로컬상의 현재틱의 Matrix 
			//_vector vDis = vTranslation - XMVectorSet(0.f, 0.f, 0.f, 0.f);//XMLoadFloat3(&m_KeyFrames[0]->vTranslation);
			_vector vMoveVector = Matrix.r[3] - XMLoadFloat4x4(&m_PreMatrix).r[3];
			m_pHierarchyNode->Set_MoveVector(vMoveVector);

			XMStoreFloat4x4(&m_PreMatrix, Matrix);

			Matrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		}
		Matrix.r[3] = XMVectorSetW(Matrix.r[3], 1.f);
		m_pHierarchyNode->Set_TransformationMatrix(Matrix);
	}

}


void CChannel::Compute_MonsterTransformationMatrix(_double PlayTime)
{
	while (m_iCurrentKeyFrame + 1 < m_KeyFrames.size())
	{
		if (PlayTime > m_KeyFrames[m_iCurrentKeyFrame + 1]->Time)
		{
			++m_iCurrentKeyFrame;
		}
		else
		{
			break;
		}
	}
	KEYFRAME* pLastKeyFrame = m_KeyFrames.back();

	_vector vScale, vRotation, vTranslation;
	_matrix CurrentMatrix;
	if (PlayTime > pLastKeyFrame->Time)
	{
		vScale = XMLoadFloat3(&pLastKeyFrame->vScale);
		vRotation = XMLoadFloat4(&pLastKeyFrame->vRotation);
		vTranslation = XMLoadFloat3(&pLastKeyFrame->vTranslation);
		vTranslation = XMVectorSetW(vTranslation, 1.f);

		CurrentMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	}
	else
	{
		_double Ratio = (PlayTime - m_KeyFrames[m_iCurrentKeyFrame]->Time) / (m_KeyFrames[m_iCurrentKeyFrame + 1]->Time - m_KeyFrames[m_iCurrentKeyFrame]->Time);

		_vector vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame]->vScale);
		_vector vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame]->vRotation);
		_vector vSourTranslation = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame]->vTranslation);

		CurrentMatrix = XMMatrixAffineTransformation(vSourScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vSourRotation, vSourTranslation); //현재 키프레임 

		_vector vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vScale);
		_vector vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vRotation);
		_vector vDestTranslation = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vTranslation);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, (_float)Ratio);
	}
	_matrix Matrix;

	if (nullptr != m_pHierarchyNode)
	{
		Matrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);  //현재 내 로컬상의 최종 매트릭스 이동값 안죽인것

		if (!strcmp("ABone", m_pHierarchyNode->Get_Name()))
		{
			if (!m_bStart)
			{
				XMStoreFloat4x4(&m_PreMatrix, Matrix);
				m_bStart = true;
			}

			//if (!XMVector3Equal(XMLoadFloat4x4(&m_PreMatrix).r[3], Matrix.r[3]))
			//{
			//	XMStoreFloat4x4(&m_PreMatrix, Matrix);
			//}

			//m_pHierarchyNode->Set_CurrentFrameMatrix(CurrentMatrix);// 전애니메이션 틱의 Matrix 를 저장해서 쓰자.  //지금은 0번쨰 매트릭스를 던
			//m_pHierarchyNode->Set_OriginTransMatrix(Matrix);        // 로컬상의 현재틱의 Matrix 
			//_vector vDis = vTranslation - XMVectorSet(0.f, 0.f, 0.f, 0.f);//XMLoadFloat3(&m_KeyFrames[0]->vTranslation);
			_vector vMoveVector = Matrix.r[3] - XMLoadFloat4x4(&m_PreMatrix).r[3];
			m_pHierarchyNode->Set_MoveVector(vMoveVector);

			XMStoreFloat4x4(&m_PreMatrix, Matrix);

			Matrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		}
		Matrix.r[3] = XMVectorSetW(Matrix.r[3], 1.f);
		m_pHierarchyNode->Set_TransformationMatrix(Matrix);
	}

}

KEYFRAME* CChannel::Get_KeyFrame(_uint iKeyFrame)
{
	return m_KeyFrames[iKeyFrame];
}


CHierarchyNode* CChannel::Get_HierarchyNode()
{
	return m_pHierarchyNode;
}

void CChannel::SetUp_HierarchyNodePtr(CHierarchyNode* pNode)
{
	m_pHierarchyNode = pNode;
	Safe_AddRef(m_pHierarchyNode);
}

void CChannel::Set_CurrentKeyFrame(_uint iKeyFrame)
{
	m_iCurrentKeyFrame = iKeyFrame;
}

HRESULT CChannel::Ready_KeyFrames(aiNodeAnim* pAIChannel)
{
	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	m_pData->NumKeyFrames = m_iNumKeyFrames;

	_float3			vScale;
	_float4			vRotation;
	_float3			vTranslation;

	m_pData->KeyFrame = new KEYFRAME[m_iNumKeyFrames];

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME*			pKeyFrame = new KEYFRAME;
		ZeroMemory(pKeyFrame, sizeof(KEYFRAME));

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			pKeyFrame->Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			pKeyFrame->Time = pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			pKeyFrame->Time = pAIChannel->mPositionKeys[i].mTime;
		}

		pKeyFrame->vScale = vScale;
		pKeyFrame->vRotation = vRotation;
		pKeyFrame->vTranslation = vTranslation;

		m_KeyFrames.push_back(pKeyFrame);
		memcpy(&m_pData->KeyFrame[i], pKeyFrame, sizeof(KEYFRAME));
	}

	return S_OK;
}

HRESULT CChannel::Ready_KeyFrames(CHANNELDESC * pData)
{
	m_iNumKeyFrames = pData->NumKeyFrames;

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME*			pKeyFrame = new KEYFRAME;
		ZeroMemory(pKeyFrame, sizeof(KEYFRAME));

		memcpy(pKeyFrame, &pData->KeyFrame[i], sizeof(KEYFRAME));

		m_KeyFrames.push_back(pKeyFrame);
	}

	return S_OK;
}

CChannel* CChannel::Create(aiNodeAnim* pAIChannel)
{
	CChannel*	pInstance = new CChannel();
	if (FAILED(pInstance->NativeConstruct_Prototype(pAIChannel)))
	{
		MSG_BOX(L"CChannel -> Create -> pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CChannel * CChannel::Create(CHANNELDESC * pData)
{
	CChannel*	pInstance = new CChannel();

	if (FAILED(pInstance->NativeConstruct_Prototype(pData)))
	{
		MSG_BOX(TEXT("Failed to Created CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Clone()
{
	CChannel*	pInstance = new CChannel(*this);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(L"CChannel -> Clone -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	if (m_pData != nullptr) {
		Safe_Delete_Array(m_pData->KeyFrame);
		Safe_Delete(m_pData);
	}

	if (false == m_isCloned)
	{
		for (auto pKeyFrame : m_KeyFrames)
			Safe_Delete(pKeyFrame);
	}

	Safe_Release(m_pHierarchyNode);
}