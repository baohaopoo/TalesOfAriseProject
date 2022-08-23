#include "..\Public\QuadTree.h"
#include "Frustum.h"
#include "PIpeLine.h"

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::NativeConstruct(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	m_iCorner[CORNER_LT] = iLT;
	m_iCorner[CORNER_RT] = iRT;
	m_iCorner[CORNER_RB] = iRB;
	m_iCorner[CORNER_LB] = iLB;

	if (1 == iRT - iLT)
		return S_OK;

	m_iCenter = (iLT + iRB) >> 1;

	_uint			iLC, iTC, iRC, iBC;

	iLC = (iLT + iLB) >> 1;
	iTC = (iLT + iRT) >> 1;
	iRC = (iRT + iRB) >> 1;
	iBC = (iLB + iRB) >> 1;

	m_pChild[CHILD_LT] = CQuadTree::Create(iLT, iTC, m_iCenter, iLC);
	m_pChild[CHILD_RT] = CQuadTree::Create(iTC, iRT, iRC, m_iCenter);
	m_pChild[CHILD_RB] = CQuadTree::Create(m_iCenter, iRC, iRB, iBC);
	m_pChild[CHILD_LB] = CQuadTree::Create(iLC, m_iCenter, iBC, iLB);

	if (FAILED(SetUp_Neighbor()))
		return S_OK;

	return S_OK;
}

void CQuadTree::Culling(CFrustum* pFrustum, _float3* pVertices, FACELISTINDICES32* pIndices, _uint* pNumFaces)
{
	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	_vector		vCamPosition = pPipeLine->Get_CamPosition();

	RELEASE_INSTANCE(CPipeLine);

	if (nullptr == m_pChild[CHILD_LT] || 
		true == LevelofDetail(pVertices, vCamPosition))
	{
		_bool		isInFrustum[CORNER_END] = {
				pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVertices[m_iCorner[CORNER_LT]]), 0.f), 
				pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVertices[m_iCorner[CORNER_RT]]), 0.f),
				pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVertices[m_iCorner[CORNER_RB]]), 0.f),
				pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVertices[m_iCorner[CORNER_LB]]), 0.f),
			};


		_bool		isDraw[NEIGHBOR_END] = { true, true, true, true };

		if(nullptr != m_pNeighbor[NEIGHBOR_LEFT])
			isDraw[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->LevelofDetail(pVertices, vCamPosition);

		if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
			isDraw[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->LevelofDetail(pVertices, vCamPosition);

		if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
			isDraw[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->LevelofDetail(pVertices, vCamPosition);

		if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
			isDraw[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->LevelofDetail(pVertices, vCamPosition);

		if (true == isDraw[NEIGHBOR_LEFT] && 
			true == isDraw[NEIGHBOR_TOP] &&
			true == isDraw[NEIGHBOR_RIGHT] && 
			true == isDraw[NEIGHBOR_BOTTOM])
		{
			if (true == isInFrustum[CORNER_LT] ||
				true == isInFrustum[CORNER_RT] ||
				true == isInFrustum[CORNER_RB])
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCorner[CORNER_RT];
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_RB];
				++*pNumFaces;
			}

			if (true == isInFrustum[CORNER_LT] ||
				true == isInFrustum[CORNER_RB] ||
				true == isInFrustum[CORNER_LB])
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCorner[CORNER_RB];
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_LB];
				++*pNumFaces;
			}

			return;
		}

		_uint			iLC, iTC, iRC, iBC;

		iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;
		iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;
		iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;
		iBC = (m_iCorner[CORNER_LB] + m_iCorner[CORNER_RB]) >> 1;

		if (true == isInFrustum[CORNER_LT] ||
			true == isInFrustum[CORNER_RB] ||
			true == isInFrustum[CORNER_LB])
		{
			if (false == isDraw[NEIGHBOR_LEFT])
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = iLC;
				++*pNumFaces;
				pIndices[*pNumFaces]._0 = iLC;
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_LB];
				++*pNumFaces;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_LB];
				++*pNumFaces;
			}

			if (false == isDraw[NEIGHBOR_BOTTOM])
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LB];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = iBC;
				++*pNumFaces;
				pIndices[*pNumFaces]._0 = iBC;
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_RB];
				++*pNumFaces;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LB];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_RB];
				++*pNumFaces;
			}
		}

		if (true == isInFrustum[CORNER_LT] ||
			true == isInFrustum[CORNER_RT] ||
			true == isInFrustum[CORNER_RB])
		{

			if (false == isDraw[NEIGHBOR_TOP])
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LT];
				pIndices[*pNumFaces]._1 = iTC;
				pIndices[*pNumFaces]._2 = m_iCenter;
				++*pNumFaces;
				pIndices[*pNumFaces]._0 = m_iCenter;
				pIndices[*pNumFaces]._1 = iTC;
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_RT];
				++*pNumFaces;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCorner[CORNER_RT];
				pIndices[*pNumFaces]._2 = m_iCenter;
				++*pNumFaces;
			}
			if (false == isDraw[NEIGHBOR_RIGHT])
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_RT];
				pIndices[*pNumFaces]._1 = iRC;
				pIndices[*pNumFaces]._2 = m_iCenter;
				++*pNumFaces;
				pIndices[*pNumFaces]._0 = m_iCenter;
				pIndices[*pNumFaces]._1 = iRC;
				pIndices[*pNumFaces]._2 = m_iCorner[CORNER_RB];
				++*pNumFaces;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorner[CORNER_RT];
				pIndices[*pNumFaces]._1 = m_iCorner[CORNER_RB];
				pIndices[*pNumFaces]._2 = m_iCenter;
				++*pNumFaces;
			}
		}

		return;
	}


	_float		fRange = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVertices[m_iCorner[CORNER_LT]]) - XMLoadFloat3(&pVertices[m_iCenter])));

	if (true == pFrustum->isIn_LocalSpace(XMLoadFloat3(&pVertices[m_iCenter]), fRange))
	{
		for (auto& pChild : m_pChild)
		{
			if (nullptr != pChild)
				pChild->Culling(pFrustum, pVertices, pIndices, pNumFaces);
		}
	}

	
}

_bool CQuadTree::LevelofDetail(_float3 * pVertices, _fvector vCamPosition)
{
	_float		fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVertices[m_iCenter]) - vCamPosition));

	_float		fWidth = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVertices[m_iCorner[CORNER_RT]]) - XMLoadFloat3(&pVertices[m_iCorner[CORNER_LT]])));

	if (fDistance * 0.1f > fWidth)
		return true;
	
	return false;
}

HRESULT CQuadTree::SetUp_Neighbor()
{	
	if (nullptr == m_pChild[CHILD_LT]->m_pChild[CHILD_LT])	
		return S_OK;	

	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RT];
	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_LB];

	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LT];
	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_RB];

	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LB];
	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_RT];

	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RB];
	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_LT];

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
	{
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] = 
			m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_LB];

		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] = 
			m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RT];
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
	{
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_RT];
	}

	for (auto& pChild : m_pChild)
	{
		if (nullptr != pChild)
			pChild->SetUp_Neighbor();
	}

	return S_OK;
}

CQuadTree * CQuadTree::Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	CQuadTree*	pInstance = new CQuadTree();

	if (FAILED(pInstance->NativeConstruct(iLT, iRT, iRB, iLB)))
	{
		MSG_BOX(TEXT("Failed to Created CQuadTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuadTree::Free()
{
	for (auto& pChild : m_pChild)
		Safe_Release(pChild);	
}