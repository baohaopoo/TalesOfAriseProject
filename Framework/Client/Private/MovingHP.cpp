#include "stdafx.h"
#include "MovingHP.h"
#include "GameInstance.h"
#include"UI_Manager.h"
#include"Player_Manager.h"
#include"Player.h"
#include"Alphen.h"
#include"Shionne.h"
MovingHP::MovingHP(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

MovingHP::MovingHP(const MovingHP & rhs)
	: CGameObject(rhs)	
	, m_ProjMatrix(rhs.m_ProjMatrix)
	
{
}

HRESULT MovingHP::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;	

	return S_OK;
}

HRESULT MovingHP::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC      TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	TargetTransform = ((MHDESC*)pArg)->targetTransform;
	m_pivotx = ((MHDESC*)pArg)->pivotx;
	m_pivoty = ((MHDESC*)pArg)->pivoty;

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	
	return S_OK;


}

void MovingHP::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();



	//카메라랑 AUTO tARGET의 위치 받아와서 거리에 따라 auto target을 그렸다 껐다. 하자.

	CTransform* CameraTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform"));

	_vector      vCameraForward = CameraTransform->Get_State(CTransform::STATE_LOOK);
	_vector		 vCameraPosition = CameraTransform->Get_State(CTransform::STATE_POSITION);
	_vector      vTarget = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - vCameraPosition);

	_float fDot = XMVectorGetX(XMVector3Dot(vCameraForward, vTarget));

	if (fDot >= 0)			// 적이 앞에 있다				
		m_bRender = true;

	else
		m_bRender = false;


	    
	_matrix ViewMatrix, ProjMatrix;
	_float4x4 WindowMatrix, WorldMatrix;

	ViewMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);

	//이곳을 주목하라.

	XMStoreFloat4x4(&WorldMatrix, TargetTransform->Get_WorldMatrix());

	ProjMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	XMStoreFloat4x4(&WindowMatrix, XMLoadFloat4x4(&WorldMatrix)*ViewMatrix*ProjMatrix);


	_float3 vWindowPos = *(_float3*)&WindowMatrix.m[3][0];

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;

	m_fX = vWindowPos.x*g_iWinCX / 2 + g_iWinCX / 2 + m_pivotx;
	m_fY = vWindowPos.y*-1 * g_iWinCY / 2 + g_iWinCY / 2 + m_pivoty;

	m_pTransformCom->Scaled(_float3(3000.f, 3000.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f, 1.f));


	CUI_Manager* pUiManager = CUI_Manager::GetInstance();
	CPlayer_Manager* pPlayerManager = CPlayer_Manager::GetInstance();
	CPlayer* player = pUiManager->GetPlayer(pPlayerManager->Get_MainPlayerIndex());
	
	Safe_AddRef(pUiManager);
	Safe_AddRef(pPlayerManager);

	if (pPlayerManager->Get_MainPlayerIndex() == 0)
	{
		hp = dynamic_cast<CAlphen*>(player)->Get_PlayerInfo().m_iCurrentHp;

		percent = hp / dynamic_cast<CAlphen*>(player)->Get_PlayerInfo().m_iMaxHp;

	}
	if (pPlayerManager->Get_MainPlayerIndex() == 1)
	{
		hp = dynamic_cast<CShionne*>(player)->Get_PlayerInfo().m_iCurrentHp;

		percent = hp / dynamic_cast<CAlphen*>(player)->Get_PlayerInfo().m_iMaxHp;

	}

	//린웰 / 카쿠라 클래스...
	/*if (pPlayerManager->Get_MainPlayerIndex() == 2)
	{
		hp = dynamic_cast<CAlphen*>(player)->GetPlayerInfo().m_iCurrentHp;

		percent = hp / dynamic_cast<CAlphen*>(player)->GetPlayerInfo().m_iMaxHp;
	}
	if (pPlayerManager->Get_MainPlayerIndex() == 3)
	{
		hp = dynamic_cast<CAlphen*>(player)->GetPlayerInfo().m_iCurrentHp;

		percent = hp / dynamic_cast<CAlphen*>(player)->GetPlayerInfo().m_iMaxHp;
	}*/



	if (m_Redbar > percent)
	{
		m_Redbar -= TimeDelta;
	}
	if (m_Redbar < percent)
		m_Redbar = percent;





	Safe_Release(pUiManager);
	Safe_Release(pPlayerManager);

}

void MovingHP::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);

	


}

HRESULT MovingHP::Render()
{

	if (m_bRender) {
		if (nullptr == m_pShaderCom ||
			nullptr == m_pVIBufferCom)
			return E_FAIL;

		m_pShaderCom->Set_RawValue("horizontalpercent", &percent, sizeof(_float));
		m_pShaderCom->Set_RawValue("g_Redbar", &m_Redbar, sizeof(_float));
		m_pShaderCom->Set_RawValue("time", &Timegone, sizeof(bool));


		if (FAILED(__super::Render()))
			return E_FAIL;

		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;


		_float4x4   ViewMatrix;
		XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;
		_float4x4   ProjMatrixTP;
		XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;


		//윈도우 -> 다음에 직교투영
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));


		//apply 하기전에 값을 던저야하므로 begin전에 숫자를 던지자.
		if (FAILED(m_pShaderCom->Begin(21)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT MovingHP::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPBar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//Prototype_Component_Texture_GreenHP

	return S_OK;
}

MovingHP * MovingHP::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	MovingHP*	pInstance = new MovingHP(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created MovingHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * MovingHP::Clone(void * pArg)
{
	MovingHP*	pInstance = new MovingHP(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created MovingHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void MovingHP::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
