#include "stdafx.h"
#include"progressUI.h"
#include"UIHpBar.h"
CProgressUI::CProgressUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CProgressUI::CProgressUI(const CProgressUI& rhs)
	:CGameObject(rhs)
	,m_ProjMatrix(rhs.m_ProjMatrix)
	,m_fWinCX(rhs.m_fWinCX)
	,m_fWinCY(rhs.m_fWinCY)
{
}

HRESULT CProgressUI::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixIdentity());
	m_fWinCX = g_iWinCX;
	m_fWinCY = g_iWinCY;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_fWinCX, m_fWinCY, 0.f, 1.f));
	return S_OK;
}

HRESULT CProgressUI::NativeConstruct(void* pArg)
{
	ZeroMemory(&m_tUIInfo, sizeof(UIINFO));

	if (FAILED(__super::NativeConstruct(pArg)))
	{

		MSG_BOX(L"Failed To CProgressUI : NativeConstruct : NativeConstruct");
		return E_FAIL;

	
	}

	if (pArg != nullptr) 
	{
		m_tUIInfo = *(UIINFO*)pArg;
	

		
	}
	if (FAILED(SetUp_Components_Basic()))
	{
		MSG_BOX(L"Failed To CProgressUI : NativeConstruct : SetUp_Components_Basic");
		return E_FAIL;
	}
	return S_OK;
}

void CProgressUI::Tick(_double TimeDelta)
{
	m_pTransformCom->Scaled(_float3(m_tUIInfo.fCX, m_tUIInfo.fCY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tUIInfo.fX - g_iWinCX * 0.5f, -m_tUIInfo.fY + g_iWinCY * 0.5f, 0.f, 1.f));
}

void CProgressUI::LateTick(_double TimeDelta)
{
	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI, this))) 
	{
		MSG_BOX(L"Failed To CProgressUI : LateTick : m_pRenderer->Add_RenderGroup");
		return;
	}
}

HRESULT CProgressUI::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_BOX(L"Failed To CProgressUI : Render : Render");
		return E_FAIL;
	}
	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShader, "g_WorldMatrix"))) 
	{
		MSG_BOX(L"Failed To CProgressUI : Render : Render");
		return E_FAIL;
	}

	_float4x4 ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
	{
		MSG_BOX(L"Failed To CProgressUI : Render : m_pShader->Set_RawValue(g_ViewMatrix)");
		return E_FAIL;
	}
	_float4x4 ProjMatrixTP;
	XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
	{
		MSG_BOX(L"Failed To CProgressUI : Render : m_pShader->Set_RawValue(g_ProjMatrix)");
		return E_FAIL;
	}
	if (FAILED(m_pTexture->SetUp_ShaderResourceView(m_pShader, "g_Texture", m_iSprite)))
	{
		MSG_BOX(L"Failed To CProgressUI : Render : m_pShader->SetUp_ShaderResourceView(g_Texture)");
		return E_FAIL;
	}
	if (FAILED(m_pShader->Begin(m_iShaderPass)))
	{
		MSG_BOX(L"Failed To CProgressUI : Render : m_pShader->Begin");
		return E_FAIL;
	}
	if (FAILED(m_pVIBuffer->Render())) 
	{
		MSG_BOX(L"Failed To CProgressUI : Render : m_pVIBuffer->Render");
		return E_FAIL;
	}
	return S_OK;
}

CProgressUI::UIINFO CProgressUI::Get_UIInfo()
{
	return m_tUIInfo;
}

HRESULT CProgressUI::SetUp_Components_Basic()
{
	if (FAILED(__super::SetUp_Components(L"Com_Renderer", LEVEL_STATIC, L"Prototype_Component_Renderer", (CComponent**)&m_pRenderer)))
	{
		MSG_BOX(L"Failed To CProgressUI : SetUp_Components_Basic : SetUp_Component(Com_Renderer)");
		return E_FAIL;
	}
	if (FAILED(__super::SetUp_Components(L"Com_Shader", LEVEL_STATIC, L"Prototype_Component_Shader_VtxTex", (CComponent**)&m_pShader)))
	{
		MSG_BOX(L"Failed To CProgressUI : SetUp_Components_Basic : SetUp_Component(Com_Shader)");
		return E_FAIL;
	}

	if (FAILED(__super::SetUp_Components(L"Com_VIBufferUI", LEVEL_STATIC, L"Prototype_Component_VIBuffer_UI", (CComponent**)&m_pVIBuffer)))
	{
		MSG_BOX(L"Failed To CProgressUI : SetUp_Components_Basic : SetUp_Component(Com_VIBuffer)");
		return E_FAIL;
	}
	return S_OK;
}

void CProgressUI::Free()
{
	__super::Free();
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}