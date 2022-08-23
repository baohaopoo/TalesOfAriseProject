#include "stdafx.h"
#include "LineSprite.h"

//ÀüÅõ
LineSprite::LineSprite(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

LineSprite::LineSprite(const LineSprite& rhs)
	:CUI(rhs)
{
}

HRESULT LineSprite::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT LineSprite::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}	

	
	m_iSprite = ((MINIMAPDESC*)pArg)->kind;
	level = ((MINIMAPDESC*)pArg)->level;

	m_fDistanceToCamera = 100;
	
	m_iShaderPass = 3;// 16;// 3;

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}



	return S_OK;
}

void LineSprite::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstnace);
	if (level == 1)
	{

		if (m_iSprite > 24)
			m_iSprite = 15;//»ï.
		else
		{
			time += TimeDelta;
			if (time > 0.04) {
				m_iSprite++;
				time = 0;
			}
		}
	}
	else if (level == 2)
	{
		m_pTransformCom->Rotation({ 0,0,1 }, XMConvertToRadians(-lineangle));
		lineangle += TimeDelta *230;
		
	}else if (level == 3)
	{

		if (m_iSprite > 27)
			m_iSprite = 27;//»ï.
		else
		{
			time += TimeDelta;
			if (time >1) {
				m_iSprite++;
				time = 0;
			}
		}


	}
	else
	{
		if (m_iSprite > 13)
		m_bDead = true;//»ï.
		else
		{
			m_iSprite++;
		}
	}
	

	Safe_Release(pGameInstnace);
}

void LineSprite::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT LineSprite::Render()
{

	 
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	
	return S_OK;
}

HRESULT LineSprite::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_TutoLineSprite", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To LineSprite : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

LineSprite * LineSprite::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	LineSprite* pInstance = new LineSprite(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To LineSprite : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * LineSprite::Clone(void * pArg)
{
	LineSprite* pInstance = new LineSprite(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To LineSprite : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void LineSprite::Free()
{
}
