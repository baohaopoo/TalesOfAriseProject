#include "stdafx.h"
#include "..\Public\Bullet.h"


CBullet::CBullet(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)


{


}


CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs)

{

}

HRESULT CBullet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC* pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
	{
		MSG_BOX(L"Failed To CBullet : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}


	return S_OK;
}

void CBullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


}

void CBullet::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

}

HRESULT CBullet::Render()
{
	return S_OK;
}


void CBullet::Free()
{
	__super::Free();

	//스킬마다 OBB SPHERE인지 모르고 모델인지 텍스처인지 모르므로 내려서 해제할것.
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);


}