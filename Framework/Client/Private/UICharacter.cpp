#include "stdafx.h"
#include "UICharacter.h"

CUICharacter::CUICharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUICharacter::CUICharacter(const CUICharacter& rhs)
	:CUI(rhs)
{
}

HRESULT CUICharacter::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUICharacter::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	UIKind = ((UICHDESC*)pArg)->kind;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	switch (UIKind)
	{

	case 0:
		m_iSprite = 0;
		break;

	case 1:
		m_iSprite = 1;
		break;
	case 2:
		m_iSprite = 2;
		break;
	case 3:
		m_iSprite = 3;
		break;
	case 4:
		m_iSprite =4;
		break;
	case 5:
		m_iSprite = 5;
	case 6:
		m_iSprite = 6;
		break;
	case 7:
		m_iSprite = 7;
		break;
	case 8:
		m_iSprite = 8;
		break;
	case 9:
		m_iSprite = 9;
		break;

	case 10:
		m_iSprite = 10;
		break;

	case 11:
		m_iSprite = 11;
		break;

	case 12:
		m_iSprite = 12;
		break;


	case 13:
		m_iSprite = 13;
		break;
	case 14:
		m_iSprite = 14;
		break;
	case 15:
		m_iSprite = 15;
		break;
	case 16:
		m_iSprite = 16;
		break;
	case 17:
		m_iSprite = 17;
		break;

	case 18:
		m_iSprite = 18;
		break;


	case 19:
		m_iSprite = 19;
		break;


	case 20:
		m_iSprite = 20;
		break;


	case 21:
		m_iSprite = 21;
		break;


	case 22:
		m_iSprite = 22;
		break;
	case 23:
		m_iSprite = 23;
		break;

	case 24:
		m_iSprite = 24;
		break;

	case 25:
		m_iSprite = 25;
		break;

	case 26:
		m_iSprite = 26;
		break;

	case 27:
		m_iSprite = 27;
		break;
	case 28:
		m_iSprite = 28;
		break;

	case 29:
		m_iSprite = 29;
		break;

	case 30:
		m_iSprite = 30;
		break;

	case 31:
		m_iSprite = 31;
		break;

	case 32:
		m_iSprite = 32;
		break;

	case 33:
		m_iSprite = 33;
		break;
	case 34:

		//아이템 인벤토리 눌렀을떄 나오는 커서.
		m_iSprite = 34;
		break;

	default:
		break;
	}
	 

	m_iShaderPass = 25;
	m_fDistanceToCamera = 99.9999f;
	return S_OK;
}

void CUICharacter::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);
	RECT rc = { 172,131, 500, 172 };


	POINT pt = Get_Mouse(g_hWnd);
	if (UIKind == 23) {
		if (PtInRect(&rc, pt))
		{
			m_isRender = true;
		}
		else
		{
			m_isRender = false;
		}
	}
	RECT rc2 = { 180, 210, 495, 235 };

	if (UIKind == 24) {
		if (PtInRect(&rc2, pt))
		{
			m_isRender = true;
		}
		else
		{
			m_isRender = false;
		}
	}
	//인벤토리 창 눌렀을때, 
	if (UIKind == 34) {
		

		cursortime += (_float)TimeDelta;
		//171
		if (m_tUIInfo.fX > 180)
		{
			m_tUIInfo.fX--;
		}
		else if (m_tUIInfo.fX >= 171 && m_tUIInfo.fX <160)
		{
			m_tUIInfo.fX *= -1;
		}
	
	

	}

	Safe_Release(pGameInstnace);
}

void CUICharacter::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CUICharacter::Render()
{
	if (!m_isBattleOn) {

		if (m_isRender) {
			if (FAILED(__super::Render()))
			{
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT CUICharacter::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_ItemCharacter", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To CUICharacter : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUICharacter * CUICharacter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUICharacter* pInstance = new CUICharacter(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CUICharacter : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUICharacter::Clone(void * pArg)
{
	CUICharacter* pInstance = new CUICharacter(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUICharacter : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUICharacter::Free()
{
	__super::Free();

}
