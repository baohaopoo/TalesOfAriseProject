#include "..\Public\Cell.h"
#include "VIBuffer_Triangle.h"
#include "Shader.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CCell::NativeConstruct(const _float3* pPoints, _uint iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);
	XMStoreFloat3(&m_vLineDir[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLineDir[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLineDir[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
	m_iIndex = iIndex;
	return S_OK;
}

HRESULT CCell::NativeConstruct(_fvector vPosA, _fvector vPosB, _fvector vPosC, _uint iIndex)
{
	XMStoreFloat3(&m_vPoints[POINT_A], vPosA);
	XMStoreFloat3(&m_vPoints[POINT_B], vPosB);
	XMStoreFloat3(&m_vPoints[POINT_C], vPosC);

	XMStoreFloat3(&m_vLineDir[LINE_AB], vPosB - vPosA);
	XMStoreFloat3(&m_vLineDir[LINE_BC], vPosC - vPosB);
	XMStoreFloat3(&m_vLineDir[LINE_CA], vPosA - vPosC);

	m_iIndex = iIndex;
	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
		{
			return true;
		}if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
		{
			return true;
		}
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
		{
			return true;
		}if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
		{
			return true;
		}
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
		{
			return true;
		}if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
		{
			return true;
		}
	}
	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector vDestDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector vSourDir = XMVector3Normalize(XMLoadFloat4(&_float4(m_vLineDir[i].z * -1.f, 0.0f, m_vLineDir[i].x, 0.f)));

		if (0 < XMVectorGetX(XMVector3Dot(vDestDir, vSourDir)))
		{
			if (0 <= m_iNeighbor[i])
			{
				*pNeighborIndex = m_iNeighbor[i];
			}
			return false;
		}
	}
	return true;
}

_uint CCell::Get_Index()
{
	return m_iIndex;
}

_vector CCell::Get_Point(POINTS ePoint)
{
	return XMLoadFloat3(&m_vPoints[ePoint]);
}

void CCell::Set_NeighborIndex(LINES eLine, _int iNeighborIndex)
{
	m_iNeighbor[eLine] = iNeighborIndex;
}

#ifdef _DEBUG
HRESULT CCell::Render(CVIBuffer_Triangle* pVIBuffer, CShader* pShader, _float4 vColor)
{
	pVIBuffer->Update(m_vPoints);
	pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
	if (FAILED(pVIBuffer->Render(pShader, 0)))
	{
		MSG_BOX(L"Failed To CCell : Render : pVIBuffer->Render");
		return E_FAIL;
	}
	return S_OK;
}
#endif // _DEBUG

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex)
{
	CCell* pInstance = new CCell(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
	{
		MSG_BOX(L"Failed To CCell : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _fvector vPosA, _fvector vPosB, _fvector vPosC, _uint iIndex)
{
	CCell* pInstance = new CCell(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct(vPosA, vPosB, vPosC, iIndex)))
	{
		MSG_BOX(L"Failed To CCell : Create : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}