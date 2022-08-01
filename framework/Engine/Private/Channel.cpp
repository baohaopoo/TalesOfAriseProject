#include "..\Public\Channel.h"

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
	strcpy_s(m_szName, pAIChannel->mNodeName.data);
	if (FAILED(Ready_KeyFrames(pAIChannel)))
	{
		MSG_BOX(L"CChannel -> NativeConstruct_Prototype -> Ready_KeyFrames");
		return E_FAIL;
	}
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

		CurrentMatrix = XMMatrixAffineTransformation(vSourScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vSourRotation, vSourTranslation);

		_vector vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vScale);
		_vector vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vRotation);
		_vector vDestTranslation = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrame + 1]->vTranslation);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, (_float)Ratio);
	}
	if (nullptr != m_pHierarchyNode)
	{
		_matrix Matrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
		if (!strcmp("TransN", m_pHierarchyNode->Get_Name()))
		{
			m_pHierarchyNode->Set_OriginTransMatrix(Matrix);
			m_pHierarchyNode->Set_CurrentFrameMatrix(CurrentMatrix);
			//_vector vDis = vTranslation - XMVectorSet(0.f, 0.f, 0.f, 0.f);//XMLoadFloat3(&m_KeyFrames[0]->vTranslation);
			Matrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		}
		Matrix.r[3] = XMVectorSetW(Matrix.r[3], 1.f);
		m_pHierarchyNode->Set_TransformationMatrix(Matrix);
	}
}
_uint CChannel::Get_CurrentKeyFrameIndex()
{
	return m_iCurrentKeyFrame;
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

	_float3 vScale;
	_float4 vRotation;
	_float3 vTranslation;

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME* pKeyFrame = new KEYFRAME;
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
	if (false == m_isCloned)
	{
		for (auto pKeyFrame : m_KeyFrames)
		{
			Safe_Delete(pKeyFrame);
		}
	}
	Safe_Release(m_pHierarchyNode);
}