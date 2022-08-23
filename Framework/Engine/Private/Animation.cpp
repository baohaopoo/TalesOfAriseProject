#include "..\Public\Animation.h"

#include "Channel.h"
#include "HierarchyNode.h"

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

	m_pData = new ANIMATIONDESC;

	strcpy_s(m_pData->Name, m_szName);
	m_pData->Duration = m_Duration;
	m_pData->TickPerSecond = m_TickPerSecond;

	if (FAILED(Ready_Channels(pAIAnimation, Nodes)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimation::NativeConstruct_Prototype(ANIMATIONDESC * pData, vector<class CHierarchyNode*> Nodes)
{
	strcpy_s(m_szName, pData->Name);
	m_Duration = pData->Duration;
	m_TickPerSecond = pData->TickPerSecond;

	if (FAILED(Ready_Channels(pData, Nodes)))
		return E_FAIL;

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

HRESULT CAnimation::NativeConstruct(ANIMATIONDESC * pData, vector<class CHierarchyNode*> Nodes)
{
	if (FAILED(Clone_Channels(pData, Nodes)))
		return E_FAIL;

	return S_OK;
}

void CAnimation::Update(_double TimeDelta, _bool bLoop)
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


		if (true == m_isFinished && true == bLoop)
		{
			m_TimeAcc = 0.0;
			m_Channels[i]->Set_CurrentKeyFrame(0);
		}
		m_Channels[i]->Compute_TransformationMatrix(m_TimeAcc);
	}


	if (m_isFinished)
	{

		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			if (!strcmp(m_Channels[i]->Get_HierarchyNode()->Get_Name(), "TransN"))
			{
				m_Channels[i]->Set_Start(false);
			}
		}
	}

}

void CAnimation::MonsterUpdate(_double TimeDelta, _bool bLoop)
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


		if (true == m_isFinished && true == bLoop)
		{
			m_TimeAcc = 0.0;
			m_Channels[i]->Set_CurrentKeyFrame(0);
		}
		m_Channels[i]->Compute_MonsterTransformationMatrix(m_TimeAcc);
	}


	if (m_isFinished)
	{

		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			if (!strcmp(m_Channels[i]->Get_HierarchyNode()->Get_Name(), "ABone"))
			{
				m_Channels[i]->Set_Start(false);
			}
		}
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

		for (_uint i = 0; i < m_Channels.size(); ++i)
		{
			if (!strcmp(m_Channels[i]->Get_HierarchyNode()->Get_Name(), "TransN"))
			{

				m_Channels[i]->Set_Start(false);
				break;
			}
		}


		for (_uint i = 0; i < vChannel.size(); ++i)
		{
			if (!strcmp(vChannel[i]->Get_HierarchyNode()->Get_Name(), "TransN"))
			{

				vChannel[i]->Set_Start(false);
				break;
			}
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
			//vChannel[i]->Set_Start(false);
			//vChannel[i]->Set_PreMatrixZeroTrans();
			continue;
		}

		vScale = XMVectorLerp(vScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vTranslation, vDestTranslation, (_float)Ratio);
		vTranslation = XMVectorSetW(vTranslation, 1.f);

		//for (_uint i = 0; i < m_iNumChannels; ++i)
		//{
		//	if (!strcmp(m_Channels[i]->Get_HierarchyNode()->Get_Name(), "TransN"))
		//	{
		//		m_Channels[i]->Set_PreMatrixZeroTrans();
		//	}
		//}
		//for (_uint i = 0; i < vChannel.size(); ++i)
		//{
		//	if (!strcmp(vChannel[i]->Get_HierarchyNode()->Get_Name(), "TransN"))
		//	{

		//		vChannel[i]->Set_PreMatrixZeroTrans();
		//	}
		//}

		m_Channels[i]->Get_HierarchyNode()->Set_TransformationMatrix(XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));
	}
}

void CAnimation::MonsterUpdate_NextAnim(_double TimeDelta, _double Duration, _uint iNextAnimIndex, _uint* pCurrentAnimIndex, _uint* pCurrentAnimIndexModel, vector<CChannel*> vChannel)
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

		for (_uint i = 0; i < m_Channels.size(); ++i)
		{
			if (!strcmp(m_Channels[i]->Get_HierarchyNode()->Get_Name(), "ABone"))
			{

				m_Channels[i]->Set_Start(false);
				break;
			}
		}


		for (_uint i = 0; i < vChannel.size(); ++i)
		{
			if (!strcmp(vChannel[i]->Get_HierarchyNode()->Get_Name(), "ABone"))
			{

				vChannel[i]->Set_Start(false);
				break;
			}
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

		if (!strcmp("ABone", vChannel[i]->Get_HierarchyNode()->Get_Name()))
		{
			//vChannel[i]->Set_Start(false);
			//vChannel[i]->Set_PreMatrixZeroTrans();
			continue;
		}

		vScale = XMVectorLerp(vScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vTranslation, vDestTranslation, (_float)Ratio);
		vTranslation = XMVectorSetW(vTranslation, 1.f);

		//for (_uint i = 0; i < m_iNumChannels; ++i)
		//{
		//	if (!strcmp(m_Channels[i]->Get_HierarchyNode()->Get_Name(), "TransN"))
		//	{
		//		m_Channels[i]->Set_PreMatrixZeroTrans();
		//	}
		//}
		//for (_uint i = 0; i < vChannel.size(); ++i)
		//{
		//	if (!strcmp(vChannel[i]->Get_HierarchyNode()->Get_Name(), "TransN"))
		//	{

		//		vChannel[i]->Set_PreMatrixZeroTrans();
		//	}
		//}

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

void CAnimation::Reset_Animation()
{
	m_TimeAccNext = 0.0;
	m_TimeAcc = 0.0;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{

		m_Channels[i]->Set_CurrentKeyFrame(0);

		m_Channels[i]->Compute_TransformationMatrix(m_TimeAcc);
	}
}

void CAnimation::Reset_MonsterAnimation()
{
	m_TimeAccNext = 0.0;
	m_TimeAcc = 0.0;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{

		m_Channels[i]->Set_CurrentKeyFrame(0);

		m_Channels[i]->Compute_MonsterTransformationMatrix(m_TimeAcc);
	}
}



CChannel* CAnimation::Get_MainChannel()
{
	return m_pMainChannel;
}

CChannel* CAnimation::Get_PelvisChannel()
{
	return m_pPelvisChannel;
}

CChannel * CAnimation::Get_CenterChannel()
{
	return m_pCenterChannel;
}


_matrix CAnimation::Get_MoveMatrix()
{
	return m_pMainChannel->Get_HierarchyNode()->Get_MoveMatrix();
}

_float3 CAnimation::Get_MoveVector()
{
	return m_pMainChannel->Get_HierarchyNode()->Get_MoveVector();
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
	m_pData->NumChannels = m_iNumChannels;
	m_pData->NumChannelNode = new int[m_iNumChannels];
	m_pData->ChannelDesc = new CHANNELDESC[m_iNumChannels];

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*			pAIChannel = pAIAnimation->mChannels[i];

		CChannel*			pChannel = CChannel::Create(pAIChannel);
		if (nullptr == pChannel)
			return E_FAIL;

		auto	iter = find_if(Nodes.begin(), Nodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIChannel->mNodeName.data, pNode->Get_Name());
		});

		if (iter == Nodes.end())
			return E_FAIL;

		m_pData->NumChannelNode[i] = (*iter)->Get_Index();
		pChannel->SetUp_HierarchyNodePtr(*iter);

		m_Channels.push_back(pChannel);
		m_pData->ChannelDesc[i] = pChannel->Get_Data();
	}

	return S_OK;
}

HRESULT CAnimation::Ready_Channels(ANIMATIONDESC * pData, vector<CHierarchyNode*> Nodes)
{
	m_iNumChannels = pData->NumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel*			pChannel = CChannel::Create(&pData->ChannelDesc[i]);
		if (nullptr == pChannel)
			return E_FAIL;
		int temp = pData->NumChannelNode[i];
		pChannel->SetUp_HierarchyNodePtr(Nodes[pData->NumChannelNode[i]]);

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

HRESULT CAnimation::Clone_Channels(aiAnimation* pAIAnimation, vector<CHierarchyNode*> Nodes)
{
	m_iNumChannels = pAIAnimation->mNumChannels;

	vector<CChannel*>		Channels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*			pAIChannel = pAIAnimation->mChannels[i];

		CChannel*			pChannel = m_Channels[i]->Clone();
		if (nullptr == pChannel)
			return E_FAIL;

		auto	iter = find_if(Nodes.begin(), Nodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pAIChannel->mNodeName.data, pNode->Get_Name());
		});

		if (iter == Nodes.end())
			return E_FAIL;

		if (!strcmp(pAIChannel->mNodeName.data, "TransN"))
		{
			m_pMainChannel = pChannel;
		}

		if (!strcmp(pAIChannel->mNodeName.data, "ABone"))
		{
			m_pMainChannel = pChannel;
		}

		if (!strcmp(pAIChannel->mNodeName.data, "pelvis"))
		{
			m_pPelvisChannel = pChannel;
		}

		if (!strcmp(pAIChannel->mNodeName.data, "CENTER_C"))
		{
			m_pCenterChannel = pChannel;
		}

		pChannel->SetUp_HierarchyNodePtr(*iter);

		Channels.push_back(pChannel);

		Safe_Release(m_Channels[i]);
	}

	m_Channels.clear();

	m_Channels = Channels;

	return S_OK;
}



HRESULT CAnimation::Clone_Channels(ANIMATIONDESC * pData, vector<CHierarchyNode*> Nodes)
{
	m_iNumChannels = pData->NumChannels;

	vector<CChannel*>		Channels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel*			pChannel = m_Channels[i]->Clone();
		if (nullptr == pChannel)
			return E_FAIL;

		pChannel->SetUp_HierarchyNodePtr(Nodes[pData->NumChannelNode[i]]);

		if (!strcmp(pChannel->Get_HierarchyNode()->Get_Name(), "TransN"))
		{
			m_pMainChannel = pChannel;
		}

		if (!strcmp(pChannel->Get_HierarchyNode()->Get_Name(), "ABone"))
		{
			m_pMainChannel = pChannel;
		}

		if (!strcmp(pChannel->Get_HierarchyNode()->Get_Name(), "pelvis"))
		{
			m_pPelvisChannel = pChannel;
		}

		if (!strcmp(pChannel->Get_HierarchyNode()->Get_Name(), "CENTER_C"))
		{
			m_pCenterChannel = pChannel;
		}

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

CAnimation * CAnimation::Create(ANIMATIONDESC * pData, vector<CHierarchyNode*> Nodes)
{
	CAnimation*	pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct_Prototype(pData, Nodes)))
	{
		MSG_BOX(TEXT("Failed to Created CAnimation"));
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

CAnimation * CAnimation::Clone(ANIMATIONDESC * pData, vector<CHierarchyNode*> Nodes)
{
	CAnimation*	pInstance = new CAnimation(*this);

	if (FAILED(pInstance->NativeConstruct(pData, Nodes)))
	{
		MSG_BOX(TEXT("Failed to Created CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	if (m_pData != nullptr) {
		Safe_Delete_Array(m_pData->NumChannelNode);
		Safe_Delete_Array(m_pData->ChannelDesc);
		Safe_Delete(m_pData);
	}

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}