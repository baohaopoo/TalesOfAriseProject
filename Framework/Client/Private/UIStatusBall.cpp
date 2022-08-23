#include"stdafx.h"
#include"UIStatusBall.h"

CUIStatusBall::CUIStatusBall(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUIStatusBall::CUIStatusBall(const CUIStatusBall& rhs)
	: CUI(rhs)
{
}

HRESULT CUIStatusBall::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}



	return S_OK;
}

HRESULT CUIStatusBall::NativeConstruct(void * pArg)
{


	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}


	m_tUIInfo.fX = 1851;
	m_tUIInfo.fY = g_iWinCY / 2 - 89;
	m_tUIInfo.fCX = 45;
	m_tUIInfo.fCY = 45;

	m_pTransformCom->Rotation({ 0,0,1 }, 180);
	m_fDistanceToCamera = 89.3;
	m_iShaderPass = 3;

	return S_OK;
}

void CUIStatusBall::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);



}

void CUIStatusBall::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CUIStatusBall::Render()
{

		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	
	return S_OK;
}

HRESULT CUIStatusBall::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_StatusBall", (CComponent**)&m_pTextureCom)))
	{


		MSG_BOX(L"Failed To UIName : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUIStatusBall * CUIStatusBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUIStatusBall* pInstance = new CUIStatusBall(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINAme : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUIStatusBall::Clone(void * pArg)
{
	CUIStatusBall* pInstance = new CUIStatusBall(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIName : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIStatusBall::Free()
{
	__super::Free();

}
