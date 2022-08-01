#pragma once
#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;
public:
	HRESULT NativeConstruct(const _float3* pPoints, _uint iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);
public:
	_uint Get_Index();
	_vector Get_Point(POINTS ePoint);
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
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END