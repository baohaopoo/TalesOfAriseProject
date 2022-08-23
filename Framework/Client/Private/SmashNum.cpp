#include "stdafx.h"
#include"SmashNum.h"
//이게 이제 버튼 누르면 반짝거리는 이미지임.
SmashNum::SmashNum(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CButton_Super(pDevice, pDeviceContext)
{
}

SmashNum::SmashNum(const SmashNum& rhs)
	: CButton_Super(rhs)
{
}

HRESULT SmashNum::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT SmashNum::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	kind = ((UISMNUMDESC*)pArg)->kind;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}



	m_iShaderPass = 3; //0
	m_fDistanceToCamera = 120;

	return S_OK;
}

void SmashNum::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstnace);
	switch (kind)
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
		m_iSprite = 4;
		break;

	case 5:
		m_iSprite = 5;//R
		break;

	case 6:
		m_iSprite = 6;//e
		break;

	case 7:
		m_iSprite = 7; //F
		break;
	case 8:
		m_iSprite = 8; //화살표
		break;
	case 9:
		m_iSprite = 9; 
		break;
	case 10:
		m_iSprite = 10;
		break;
	}

	//얼굴.. 
	if (kind == 9)
	{

		charTalk1Time += (_float)TimeDelta;

		if (m_tUIInfo.fX >= 110)
		{
			m_tUIInfo.fX = 110;
		}
		else m_tUIInfo.fX += 10;

		if (charTalk1Time > 3)
		{
			m_tUIInfo.fX = 0;
			charTalk1Time = 0;
		}

	}
	if (kind == 10)
	{

		charTalkTime2 += (_float)TimeDelta;

		if (m_tUIInfo.fX >= 110)
		{
			m_tUIInfo.fX = 110;
		}
		else m_tUIInfo.fX += 10;

		if (charTalkTime2 > 3)
		{
			m_tUIInfo.fX = 0;
			charTalkTime2 = 0;
		}
		


	}

	Safe_Release(pGameInstnace);
}
void SmashNum::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT SmashNum::Render()
{

	if (!m_isBattleOn) {
		if (m_isRender) {
			//잠만 보류
			//if (FAILED(m_pShaderCom->Set_RawValue("ItemListrgb", &rgb, sizeof(float))))
			//	return E_FAIL;


			if (FAILED(__super::Render()))
			{
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT SmashNum::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_SmashNumTexture", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"texture오류다임마.");
		return E_FAIL;
	}
	return S_OK;	
}



SmashNum * SmashNum::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	SmashNum* pInstance = new SmashNum(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To SmashNum : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * SmashNum::Clone(void * pArg)
{
	SmashNum* pInstance = new SmashNum(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To SmashNum : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void SmashNum::Free()
{
	__super::Free();

}
