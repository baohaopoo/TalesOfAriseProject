#include "stdafx.h"
#include "..\Public\LobbyBtn.h"
#include"..\Public\Level_Loading.h"
//이게 이제 버튼 누르면 반짝거리는 이미지임.
LobbyBtn::LobbyBtn(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CButton_Super(pDevice, pDeviceContext)
{
}

LobbyBtn::LobbyBtn(const LobbyBtn& rhs)
	: CButton_Super(rhs)
{
}

HRESULT LobbyBtn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT LobbyBtn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}


	m_iShaderPass = 3;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	m_fDistanceToCamera = 90;
	return S_OK;
}

void LobbyBtn::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	RECT rc;

	if (kind == 0)
	{
		//StartBtn
		rc = { 317,633,409,666 };
	}
	if (kind == 1)
	{
		//ExitBtn
		rc = { 322,688,399,714 };


	}

	POINT pt = Get_Mouse(g_hWnd);


	if (PtInRect(&rc, pt))
	{
		m_isTouched = true;

	}
	else m_isTouched = false;


}

void LobbyBtn::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT LobbyBtn::Render()
{

	if (m_isTouched)
	{

		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}

	}

	if (m_isRender)
	{
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}


HRESULT LobbyBtn::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_BtnGlow", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"LobbyBtn 텍스쳐 셋업 오류");
		return E_FAIL;
	}

	return S_OK;
}



LobbyBtn * LobbyBtn::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	LobbyBtn* pInstance = new LobbyBtn(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To LobbyBtn : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * LobbyBtn::Clone(void * pArg)
{
	LobbyBtn* pInstance = new LobbyBtn(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To LobbyBtn : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void LobbyBtn::Free()
{
	__super::Free();

}
