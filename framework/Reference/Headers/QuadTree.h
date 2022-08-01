#pragma once

#include "Base.h"

BEGIN(Engine)

class CQuadTree final : public CBase
{
public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_RB, CHILD_LB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };
private:
	CQuadTree();
	virtual ~CQuadTree() = default;

public:
	HRESULT NativeConstruct(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	void Culling(class CFrustum* pFrustum, _float3* pVertices, FACELISTINDICES32* pIndices, _uint* pNumFaces);
	_bool LevelofDetail(_float3* pVertices, _fvector vCamPosition);

private:
	CQuadTree*			m_pChild[CHILD_END] = { nullptr };
	CQuadTree*			m_pNeighbor[NEIGHBOR_END] = { nullptr };
	_uint				m_iCorner[CORNER_END];
	_uint				m_iCenter;

public:
	HRESULT SetUp_Neighbor();

public:
	static CQuadTree* Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free() override;
};

END