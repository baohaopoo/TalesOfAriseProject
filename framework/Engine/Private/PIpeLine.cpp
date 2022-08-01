#include "..\Public\PIpeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

_float4x4 CPipeLine::Get_TransformFloat4x4_TP(TRANSFORMSTATE eState)
{
	_float4x4		TransformMatrix;

	XMStoreFloat4x4(&TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix[eState])));

	return TransformMatrix;
}

void CPipeLine::Tick()
{
	XMStoreFloat4(&m_vCamPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrix[D3DTS_VIEW])).r[3]);
}

void CPipeLine::Free()
{
}
