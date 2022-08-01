#include "..\Public\Animation.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	:m_Channels(rhs.m_Channels)
	, m_iNumChannels(rhs.m_iNumChannels)
	, m_Duration(rhs.m_Duration)
	, m_TickPerSecond(rhs.m_TickPerSecond)
	, m_TimeAcc(rhs.m_TimeAcc)
	, m_isFinished(rhs.m_isFinished)
{
	for (auto& pChannel : m_Channels)
	{
		Safe_AddRef(pChannel);
	}
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CAnimation::NativeConstruct_Prototype(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;
	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	if (FAILED(Ready_Channels(pAIAnimation, Nodes)))
	{
		MSG_BOX(L"CAnimation -> NativeConstruct_Prototype -> Ready_Channels");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CAnimation::NativeConstruct(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	if (FAILED(Clone_Channels(pAIAnimation, Nodes)))
	{
		MSG_BOX(L"CAnimation -> NativeConstruct -> Clone_Channels");
		return E_FAIL;
	}
	return S_OK;
}

void CAnimation::Update(_double TimeDelta, _bool bRoof)
{
	m_TimeAccNext = 0.0;
	m_isFinished = false;
	m_TimeAcc += m_TickPerSecond * TimeDelta;
	if (m_TimeAcc >= m_Duration)
	{
		m_isFinished = true;
	}
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		if (true == m_isFinished && true == bRoof)
		{
			m_TimeAcc = 0.0;
			m_Channels[i]->Set_CurrentKeyFrame(0);
		}
		m_Channels[i]->Compute_TransformationMatrix(m_TimeAcc);
	}
}

//void CAnimation::Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex, _uint* pCurrentAnimIndexModel, vector<CChannel*> vChannel)
//{
//	m_isFinished = false;
//	m_TimeAcc = 0.0;
//	m_TimeAccNext += m_TickPerSecond * TimeDelta;
//	if (m_TimeAccNext >= Duration)
//	{
//		m_TimeAccNext = 0.0;
//		*pCurrentAnimIndex = iNextAnimIndex;
//		*pCurrentAnimIndexModel = iNextAnimIndex;
//		for (_uint i = 0; i < m_iNumChannels; ++i)
//		{
//			m_Channels[i]->Set_CurrentKeyFrame(0);
//		}
//		return;
//	}
//
//	for (_uint i = 0; i < m_iNumChannels; ++i)
//	{
//		_double Ratio = m_TimeAccNext / Duration;
//
//		_vector vRotation, vScale, vTranslation;
//		vRotation = XMLoadFloat4(&m_Channels[i]->Get_KeyFrame(m_Channels[i]->Get_CurrentKeyFrameIndex())->vRotation);
//		vScale = XMLoadFloat3(&m_Channels[i]->Get_KeyFrame(m_Channels[i]->Get_CurrentKeyFrameIndex())->vScale);
//		vTranslation = XMLoadFloat3(&m_Channels[i]->Get_KeyFrame(m_Channels[i]->Get_CurrentKeyFrameIndex())->vTranslation);
//		vTranslation = XMVectorSetW(vTranslation, 1.f);
//
//		_vector vDestRotation, vDestScale, vDestTranslation;
//		vDestRotation = XMLoadFloat4(&vChannel[i]->Get_KeyFrame(0)->vRotation);
//		vDestScale = XMLoadFloat3(&vChannel[i]->Get_KeyFrame(0)->vScale);
//		vDestTranslation = XMLoadFloat3(&vChannel[i]->Get_KeyFrame(0)->vTranslation);
//		vDestTranslation = XMVectorSetW(vDestTranslation, 1.f);
//
//		vScale = XMVectorLerp(vScale, vDestScale, (_float)Ratio);
//		vRotation = XMQuaternionSlerp(vRotation, vDestRotation, (_float)Ratio);
//		vTranslation = XMVectorLerp(vTranslation, vDestTranslation, (_float)Ratio);
//		vTranslation = XMVectorSetW(vTranslation, 1.f);
//
//		m_Channels[i]->Get_HierarchyNode()->Set_TransformationMatrix(XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));
//	}
//}

void CAnimation::Update_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex, _uint* pCurrentAnimIndexModel, vector<CChannel*> vChannel)
{
	m_isFinished = false;
	m_TimeAcc = 0.0;
	m_TimeAccNext += m_TickPerSecond * TimeDelta;
	if (m_TimeAccNext >= Duration)
	{
		m_TimeAccNext = 0.0;
		*pCurrentAnimIndex = iNextAnimIndex;
		*pCurrentAnimIndexModel = iNextAnimIndex;
		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			m_Channels[i]->Set_CurrentKeyFrame(0);
		}
		return;
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		_double Ratio = m_TimeAccNext / Duration;

		_vector vRotation, vScale, vTranslation;
		vRotation = XMLoadFloat4(&m_Channels[i]->Get_KeyFrame(m_Channels[i]->Get_CurrentKeyFrameIndex())->vRotation);
		vScale = XMLoadFloat3(&m_Channels[i]->Get_KeyFrame(m_Channels[i]->Get_CurrentKeyFrameIndex())->vScale);
		vTranslation = XMLoadFloat3(&m_Channels[i]->Get_KeyFrame(m_Channels[i]->Get_CurrentKeyFrameIndex())->vTranslation);
		vTranslation = XMVectorSetW(vTranslation, 1.f);

		_vector vDestRotation, vDestScale, vDestTranslation;
		vDestRotation = XMLoadFloat4(&vChannel[i]->Get_KeyFrame(0)->vRotation);
		vDestScale = XMLoadFloat3(&vChannel[i]->Get_KeyFrame(0)->vScale);
		vDestTranslation = XMLoadFloat3(&vChannel[i]->Get_KeyFrame(0)->vTranslation);
		vDestTranslation = XMVectorSetW(vDestTranslation, 1.f);

		if (!strcmp("TransN", vChannel[i]->Get_HierarchyNode()->Get_Name()))
		{
			continue;
		}

		vScale = XMVectorLerp(vScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vTranslation, vDestTranslation, (_float)Ratio);
		vTranslation = XMVectorSetW(vTranslation, 1.f);

		m_Channels[i]->Get_HierarchyNode()->Set_TransformationMatrix(XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));
	}
}

vector<CChannel*> CAnimation::Get_Channels()
{
	return m_Channels;
}

_bool CAnimation::Get_Finish()
{
	return m_isFinished;
}

_double CAnimation::Get_TimeAcc()
{
	return m_TimeAcc;
}

_matrix CAnimation::Get_MoveMatrix()
{
	return m_pMainChannel->Get_HierarchyNode()->Get_MoveMatrix();
}

void CAnimation::Set_ZeroFrame()
{
	m_TimeAcc = 0.0;
	m_isFinished = false;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Set_CurrentKeyFrame(0);
	}
}

HRESULT CAnimation::Ready_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	m_iNumChannels = pAIAnimation->mNumChannels;
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim* pAIChannel = pAIAnimation->mChannels[i];

		CChannel* pChannel = CChannel::Create(pAIChannel);
		if (nullptr == pChannel)
		{
			MSG_BOX(L"CAnimation -> Ready_Channels -> nullptr == pChannel");
			return E_FAIL;
		}
		auto iter = find_if(Nodes.begin(), Nodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIChannel->mNodeName.data, pNode->Get_Name());
		});

		if (iter == Nodes.end())
		{
			MSG_BOX(L"CAnimation -> Ready_Channels -> iter == Nodes.end");
			return E_FAIL;
		}
		pChannel->SetUp_HierarchyNodePtr(*iter);
		m_Channels.push_back(pChannel);
	}
	return S_OK;
}

HRESULT CAnimation::Clone_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	m_iNumChannels = pAIAnimation->mNumChannels;
	vector<CChannel*> Channels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim* pAIChannel = pAIAnimation->mChannels[i];

		CChannel* pChannel = m_Channels[i]->Clone();
		if (nullptr == pChannel)
		{
			MSG_BOX(L"CAnimation -> Clone_Channels -> nullptr == pChannel");
			return E_FAIL;
		}
		auto iter = find_if(Nodes.begin(), Nodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIChannel->mNodeName.data, pNode->Get_Name());
		});

		if (iter == Nodes.end())
		{
			MSG_BOX(L"CAnimation -> Clone_Channels -> iter == Nodes.end");
			return E_FAIL;
		}

		if (!strcmp(pAIChannel->mNodeName.data, "TransN"))
		{
			m_pMainChannel = pChannel;
		}

		pChannel->SetUp_HierarchyNodePtr(*iter);
		Channels.push_back(pChannel);
		Safe_Release(m_Channels[i]);
	}
	m_Channels.clear();
	m_Channels = Channels;
	return S_OK;
}

CAnimation* CAnimation::Create(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	CAnimation*	pInstance = new CAnimation();
	if (FAILED(pInstance->NativeConstruct_Prototype(pAIAnimation, Nodes)))
	{
		MSG_BOX(L"CAnimation -> Create -> pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation* CAnimation::Clone(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	CAnimation*	pInstance = new CAnimation(*this);
	if (FAILED(pInstance->NativeConstruct(pAIAnimation, Nodes)))
	{
		MSG_BOX(L"CAnimation -> Clone -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
	{
		Safe_Release(pChannel);
	}
	m_Channels.clear();
}