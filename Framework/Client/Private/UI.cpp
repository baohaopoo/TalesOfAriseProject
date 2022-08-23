#include "stdafx.h"
#include "..\Public\UI.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
	, m_fWinCX(rhs.m_fWinCX)
	, m_fWinCY(rhs.m_fWinCY)
{
}

HRESULT CUI::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixIdentity());
	m_fWinCX = g_iWinCX;
	m_fWinCY = g_iWinCY;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_fWinCX, m_fWinCY, 0.f, 1.f));
	return S_OK;
}

HRESULT CUI::NativeConstruct(void* pArg)
{
	ZeroMemory(&m_tUIInfo, sizeof(UIINFO));

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUI : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}

	if (pArg != nullptr)
	{
		m_tUIInfo = *(UIINFO*)pArg;
	}

	if (FAILED(SetUp_Components_Basic()))
	{
		MSG_BOX(L"Failed To CUI : NativeConstruct : SetUp_Components_Basic");
		return E_FAIL;
	}

	return S_OK;
}

void CUI::Tick(_double TimeDelta)
{

	//rgbda += 0.08;
	m_pTransformCom->Scaled(_float3(m_tUIInfo.fCX, m_tUIInfo.fCY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tUIInfo.fX - g_iWinCX * 0.5f, -m_tUIInfo.fY + g_iWinCY * 0.5f, 0.f, 1.f));
}

void CUI::LateTick(_double TimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this)))
	{
		MSG_BOX(L"Failed To CUI : LateTick : m_pRenderer->Add_RenderGroup");
		return;
	}
}

HRESULT CUI::Render()
{

	if (m_isRender) {


		//if (FAILED(m_pShaderCom->Set_RawValue("alluirgb", &rgbda, sizeof(float))))
		//	return E_FAIL;


		//
		if (FAILED(__super::Render()))
		{
			MSG_BOX(L"Failed To CUI : Render : Render");
			return E_FAIL;
		}



		if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		{
			MSG_BOX(L"Failed To CUI : Render : Render");
			return E_FAIL;
		}

		_float4x4 ViewMatrix;
		XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		{
			MSG_BOX(L"Failed To CUI : Render : m_pShader->Set_RawValue(g_ViewMatrix)");
			return E_FAIL;
		}
		_float4x4 ProjMatrixTP;
		XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
		{
			MSG_BOX(L"Failed To CUI : Render : m_pShader->Set_RawValue(g_ProjMatrix)");
			return E_FAIL;
		}
		if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", m_iSprite)))
		{
			MSG_BOX(L"Failed To CUI : Render : m_pShader->SetUp_ShaderResourceView(g_Texture)");
			return E_FAIL;
		}

		if (FAILED(m_pShaderCom->Begin(m_iShaderPass)))
		{
			MSG_BOX(L"Failed To CUI : Render : m_pShader->Begin");
			return E_FAIL;
		}

		if (FAILED(m_pVIBufferCom->Render()))
		{
			MSG_BOX(L"Failed To CUI : Render : m_pVIBuffer->Render");
			return E_FAIL;
		}
	}
	return S_OK;
}

CUI::UIINFO CUI::Get_UIInfo()
{
	return m_tUIInfo;
}

HRESULT CUI::SetUp_Components_Basic()
{
	if (FAILED(__super::SetUp_Components(L"Com_Renderer", LEVEL_STATIC, L"Prototype_Component_Renderer", (CComponent**)&m_pRendererCom)))
	{
		MSG_BOX(L"Failed To CUI : SetUp_Components_Basic : SetUp_Component(Com_Renderer)");
		return E_FAIL;
	}
	if (FAILED(__super::SetUp_Components(L"Com_Shader", LEVEL_STATIC, L"Prototype_Component_Shader_VtxTex", (CComponent**)&m_pShaderCom)))
	{
		MSG_BOX(L"Failed To CUI : SetUp_Components_Basic : SetUp_Component(Com_Shader)");
		return E_FAIL;
	}
	if (FAILED(__super::SetUp_Components(L"Com_VIBuffer", LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", (CComponent**)&m_pVIBufferCom)))
	{
		MSG_BOX(L"Failed To CUI : SetUp_Components_Basic : SetUp_Component(Com_VIBuffer)");
		return E_FAIL;
	}
	return S_OK;
}
void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferUICom);
}