#pragma once

#include "VIBuffer.h"


BEGIN(Engine)

class CQuadTree;
class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMapPath);
	virtual HRESULT NativeConstruct(void* pArg);
public:
	virtual _float Compute_Height(class CTransform* pTransform, _fvector vPosition);

public:
	void Culling(class CTransform* pTransform);

private:
	_uint				m_iNumVerticesX = 0;
	_uint				m_iNumVerticesZ = 0;
	FACELISTINDICES32*	m_pIndices = nullptr;

private:
	class CQuadTree*	m_pQuadTree = nullptr;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pHeightMapPath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

