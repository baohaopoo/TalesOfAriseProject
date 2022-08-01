#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNaviDesc
	{
		class CTransform* pParent = nullptr;
		_uint	iCurrentIndex = -1;
	}NAVIDESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pNaviDataFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	_bool Move_OnNavigation(_fvector vPosition);
#ifdef _DEBUG
public:
	virtual HRESULT Render() override;
#endif // _DEBUG
private:
	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;
#ifdef _DEBUG
private:
	class CVIBuffer_Triangle*		m_pVIBuffer = nullptr;
	class CShader*					m_pShader = nullptr;
#endif // _DEBUG
private:
	NAVIDESC m_NaviDesc;
private:
	HRESULT SetUp_Neighbor();
public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNaviDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END