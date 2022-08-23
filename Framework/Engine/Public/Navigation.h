#pragma once
#include "Component.h"

BEGIN(Engine)

class CCell;

END



BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNaviDesc
	{
		class CTransform* pParent = nullptr;
		_uint	iCurrentIndex = -1;
		_matrix NaviWorldMatrix = XMMatrixIdentity();
	}NAVIDESC;

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pNaviDataFilePath);
	virtual HRESULT NativeConstruct_Prototype(void);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_bool Move_OnNavigation(_fvector vPosition);
	class CCell* Get_CurCell(void);
	void Find_My_Cell(_vector vPosition);

#ifdef _DEBUG
public:
	void Add_Cell(class CCell* pCell) {
		m_setCells.emplace(pCell);
	}
	void Delete_Cell(class CCell* pCell) {
		m_setCells.erase(pCell);
	}
	const set<class CCell*>& Get_setCells(void) { return m_setCells; }

public:
	virtual HRESULT Render() override;
#endif // _DEBUG

private:
	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;

#ifdef _DEBUG

private:
	set<class CCell*>				m_setCells;

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
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END