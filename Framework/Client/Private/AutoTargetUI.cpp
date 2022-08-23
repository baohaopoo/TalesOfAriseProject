#include"stdafx.h"
#include"AutoTargetUI.h"
#include "Enemy.h"
#include "Player_Manager.h"
#include "Player.h"
#include"Alphen.h"

AutoTargetUI::AutoTargetUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

AutoTargetUI::AutoTargetUI(const AutoTargetUI& rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)

{
}

HRESULT AutoTargetUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));

	return S_OK;
}

HRESULT AutoTargetUI::NativeConstruct(void * pArg)
{


	CTransform::TRANSFORMDESC      TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void AutoTargetUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CTransform* vampireTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector vampire = vampireTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vampirepos;
	XMStoreFloat3(&vampirepos, vampire);



	_matrix ViewMatrix, ProjMatrix;
	_float4x4 WindowMatrix, WorldMatrix;

	ViewMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);



	//enemy의 위치값 받아오기.
	CPlayer_Manager* pPlayerManger = CPlayer_Manager::GetInstance();
	CBattle_Manager* pBattleManager = CBattle_Manager::GetInstance();
	vector<class CEnemy*>* monstervec = pBattleManager->Get_VecMonster();

	if (monstervec->size() > 0)
	{
		CPlayer* MainPlayer = pPlayerManger->Get_MainPlayer();
		vector<CEnemy*>::iterator  Target_Iter = MainPlayer->Get_Targeting();

		_vector enemyPOS = dynamic_cast<CEnemy*>(*Target_Iter)->Get_EnemyPos();
		CTransform*enempyTransform = (CTransform*)(*Target_Iter)->Get_Component(TEXT("Com_Transform"));
		XMStoreFloat4x4(&WorldMatrix, enempyTransform->Get_WorldMatrix());


		ProjMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
		XMStoreFloat4x4(&WindowMatrix, XMLoadFloat4x4(&WorldMatrix)*ViewMatrix*ProjMatrix);


		_float3 vWindowPos = *(_float3*)&WindowMatrix.m[3][0];

		vWindowPos.x /= vWindowPos.z;
		vWindowPos.y /= vWindowPos.z;

		m_fX = vWindowPos.x * g_iWinCX / 2 + g_iWinCX / 2;
		m_fY = vWindowPos.y*-1 * g_iWinCY / 2 + g_iWinCY / 2;



		m_pTransformCom->Scaled(_float3(100, 100, 1.f));

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f, 1.f));
	}



	



}

void AutoTargetUI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);


}

HRESULT AutoTargetUI::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

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

	//apply 하기전에 값을 던저야하므로 begin전에 숫자를 던지자.
	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT AutoTargetUI::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_AutoTargetUI"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

AutoTargetUI * AutoTargetUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	AutoTargetUI* pInstance = new AutoTargetUI(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINAme : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * AutoTargetUI::Clone(void * pArg)
{
	AutoTargetUI* pInstance = new AutoTargetUI(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIName : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void AutoTargetUI::Free()
{
}
