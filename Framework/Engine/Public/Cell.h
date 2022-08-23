#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum Attribute { Att_Normal, Att_Blocked, Att_Missing1, Att_Missing2, Att_Missing3, Att_Missing4, Att_Missing5, Att_Missing6, Att_Missing7, Att_Missing8, Att_END };
	// Normal : 이동 가능
	// Blocked : 이동 불가능
	// 나머지는 미정 Att_Missing1 ~ Att_Missing8 

public:
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;

public:
	HRESULT NativeConstruct(const _float3* pPoints, _uint iIndex);
	HRESULT NativeConstruct(_fvector vPosA, _fvector vPosB, _fvector vPosC, _uint iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);

public:
	_uint Get_Index();
	_vector Get_Point(POINTS ePoint);
	_float3* Get_Points(void) { return m_vPoints; }

public:
	void Set_NeighborIndex(LINES eLine, _int iNeighborIndex);

#ifdef _DEBUG
public:
	HRESULT Render(class CVIBuffer_Triangle* pVIBuffer, class CShader* pShader, _float4	vColor);
#endif // _DEBUG

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	_int m_iNeighbor[LINE_END] = { -1, -1, -1 };
	_float3 m_vPoints[POINT_END];
	_float3 m_vLineDir[LINE_END];
	_uint m_iIndex = 0;

public:
	Attribute Get_Attribute(void) { return m_eAttrbute; }
	void Set_Attribute(Attribute eAttribute) { m_eAttrbute = eAttribute; }

private:
	Attribute m_eAttrbute;	// 셀의 속성

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex = 0);
	static CCell* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _fvector vPosA, _fvector vPosB, _fvector vPosC, _uint iIndex);
	virtual void Free() override;
};

END