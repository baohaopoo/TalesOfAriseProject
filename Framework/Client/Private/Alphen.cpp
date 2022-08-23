#include "stdafx.h"
#include "..\Public\Alphen.h"
#include "GameInstance.h"

#include "Animation.h"
#include "Channel.h"
#include "Navigation.h"

#include "Sword.h"


#include"LineMsgUI.h"
#include"UI_Manager.h"

#include"MovingHP.h"

#define JUMP_SPEED 0.4


CAlphen::CAlphen(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CPlayer(pDeviceOut, pDeviceContextOut)
{

}

CAlphen::CAlphen(const CAlphen & rhs)
	: CPlayer(rhs)
{
}

HRESULT CAlphen::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAlphen::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC      TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	

	TransformDesc.SpeedPerSec = 30.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimationIndex(m_iCurrentAnimationIndex);

	CGameInstance*pGameInstance = CGameInstance::GetInstance();
	
	CEquipment::SOCKETDESC         SocketDesc;
	ZeroMemory(&SocketDesc, sizeof(CEquipment::SOCKETDESC));

	CUI_Manager* m_pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(m_pUIManager);
	SocketDesc.pModelCom = m_pModelSKLCom;
	SocketDesc.iIndex = 100;
	SocketDesc.pBoneName = m_FlamBone;


	m_pFlamSword = (CSword*)pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_Sword"), TEXT("Prototype_GameObject_Sword"), &SocketDesc);
	if (m_pFlamSword == nullptr)
		return E_FAIL;
	vecEquipment.push_back(m_pFlamSword);
	m_pFlamSword->Set_Battle(true);
	m_pFlamSword->Set_Field(true);


	//아이템에따라 바꿀수있는 검
	SocketDesc.pModelCom = m_pModelSKLCom;
	SocketDesc.iIndex = 28;
	SocketDesc.pBoneName = m_HandBone;
	m_pNormalSword = (CSword*)pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_Sword"), TEXT("Prototype_GameObject_Sword"), &SocketDesc);
	if (m_pNormalSword == nullptr)
		return E_FAIL;
	vecEquipment.push_back(m_pNormalSword);

	m_pNormalSword->Set_Battle(true);
	m_pNormalSword->Set_Field(true);


	m_tPlayerInfo.m_iComboHits = 0;
	m_tPlayerInfo.m_iCurrentAg = 7;
	m_tPlayerInfo.m_iMaxHp = 100;
	m_tPlayerInfo.m_iMaxAg = 7;
	m_tPlayerInfo.m_iCurrentHp = 100;


	//_int m_iMaxCp = 125;
	//_int m_iCurrentCp = 125;
	//_int m_iMaxHp = 9999;
	//_int m_iCurrentHp = 9999;
	//_int m_iComboHits = 0;
	//_int m_iMaxAg = 7;
	//_int m_iCurrentAg = 7;



	m_pPlayer_Manager->Set_MainPlayer(0);
	m_pPlayer_Manager->Set_FieldPlayer(0);
	m_pUIManager->SetPlayer(0, this);
	m_bAuto = false;
	m_bBattle = false;
	m_bField = true;


	m_eSkillAirFirst = SKILL_AIR_RISING_PALCON;
	m_eSkillGroundFirst = SKILL_GROUND_LIGTHING_THRUST;

	m_eSkillAirSecond = SKILL_AIR_HURRICANE_THRUST;
	m_eSkillGroundSecond = SKILL_GROUND_SWALLOW_BLADE;

	m_eSkillAirThird = SKILL_AIR_RISING_PALCON;
	m_eSkillGroundThird = SKILL_GROUND_DRAGON_SWARM;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(119.f, 0.f, 304.f, 1.f));
	Set_PlayerPos(XMVectorSet(0.f, 0.f, 0.f, 1.));


	//MovingHP::MHDESC mhdesc;
	//mhdesc.targetTransform = m_pTransformCom;
	//mhdesc.pivoty = -30;
	//if (nullptr == pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_MovingHP"), &mhdesc))
	//	return E_FAIL;

	Safe_Release(m_pUIManager);
	return S_OK;
}

void CAlphen::Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (m_bBattle) {

		if ((m_bSkill && pGameInstance->Key_Up(DIK_R)) || (m_bSkill && pGameInstance->Key_Up(DIK_E)) || (m_bSkill && pGameInstance->Key_Up(DIK_F))) {


			LineMsgUI::MSGLINEUIDESC LineMsgDESC;
			LineMsgDESC.uidesc.fX = g_iWinCX / 2;
			LineMsgDESC.uidesc.fY = g_iWinCX / 2 - 700;
			LineMsgDESC.uidesc.fCX = 700;//700;
			LineMsgDESC.uidesc.fCY = 100;//250;
			LineMsgDESC.sprite = m_eSkillUse;//2;  //1부터 쓰는 이유가 있다.

			if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, L"Layer_BattleUI", TEXT("Prototype_GameObject_LineMsgUI"), &LineMsgDESC)))
				return;

		}
	}

	//m_tPlayerInfo.m_iCurrentHp--;


	if (m_bBattle || m_bField)
	{
		__super::Tick(TimeDelta);

		//UI확인용



		m_tPlayerInfo.m_iComboHits++;
		m_tPlayerInfo.m_iCurrentAg--;
		m_tPlayerInfo.m_iCurrentHp -= 0.00001;


		if (m_tPlayerInfo.m_iComboHits > 999)
			m_tPlayerInfo.m_iComboHits = 0;
		if (m_tPlayerInfo.m_iCurrentHp < 0)
			m_tPlayerInfo.m_iCurrentHp = m_tPlayerInfo.m_iMaxHp;
		if (m_tPlayerInfo.m_iCurrentAg < 0)
			m_tPlayerInfo.m_iCurrentAg = m_tPlayerInfo.m_iMaxAg;

		
		m_bOnAttackCollider = false;


		if (m_bField)
		{
			if (m_bBattle)
			{
				if (!m_bAuto)
				{
					if (!m_bHit)
					{
						Key_Input();

						if (!m_bStop)
							Compute_Anim(TimeDelta*m_dTimeSpeed);
					}

					else {
						if (!m_bStop)
							Compute_Hit(TimeDelta*m_dTimeSpeed);

					}
				}

				else
				{

					if (!m_bStop)
						AI(TimeDelta*m_dTimeSpeed);
					//AI구현 해야함 여기다가AI();
				}

				Compute_Attack();

			}
			else
			{
				Key_Input_Town();
				if (!m_bStop)
					Compute_Anim(TimeDelta*m_dTimeSpeed);

			}
		}

		else
		{
			if (m_bBattle)
			{
				if (!m_bAuto)
				{
					if (!m_bHit)
					{
						Key_Input();
						if (!m_bStop)
							Compute_Anim(TimeDelta*m_dTimeSpeed);
					}

					else {
						if (!m_bStop)
							Compute_Hit(TimeDelta*m_dTimeSpeed);

					}
				}
				else
				{
					if (!m_bStop)
						AI(TimeDelta*m_dTimeSpeed);
					//AI구현 해야함 여기다가AI();
				}
			}
		}








		/*Compute_Move(TimeDelta);
		Skill();
		Jump(TimeDelta);
		*/

		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
		m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
		m_pAttackSphereCom->Update(XMMatrixIdentity());  //항등넣어야함 


		
		if (pGameInstance->Key_Down(DIK_T))
		{
			m_bDamage_Small_b = true;
			m_bHit = true;
		}

	
	}
}

void CAlphen::LateTick(_double TimeDelta)
{
	if (m_bBattle || m_bField)
	{

		if (m_bAttackCollision)
		{

			m_bHit = true;
			Compute_HitInfo();

			m_bAttackCollision = false;
		}

		__super::LateTick(TimeDelta);
		CGameInstance*      pGameInstance = GET_INSTANCE(CGameInstance);

#ifdef _DEBUG
		m_pRendererCom->Add_DebugComponent(m_pOBBCom);
		m_pRendererCom->Add_DebugComponent(m_pSphereCom);
		m_pRendererCom->Add_DebugComponent(m_pAttackSphereCom);
#endif // _DEBUG
		RELEASE_INSTANCE(CGameInstance);
	}
}

HRESULT CAlphen::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint      iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;
		//아직 노멀 텍스쳐가 없어서 띄울수가 없다
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;

	}

	_uint      iNumFaceMeshContainers = m_pModelFaceCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumFaceMeshContainers; ++i)
	{
		if (FAILED(m_pModelFaceCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelFaceCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}


	_uint      iNumHairMeshContainers = m_pModelHairCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumHairMeshContainers; ++i)
	{
		if (FAILED(m_pModelHairCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelHairCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelHairCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}


	m_pOBBCom->Render();
	m_pSphereCom->Render();
	if (m_bOnAttackCollider)
	{
		m_pAttackSphereCom->Render();
	}

	return S_OK;
}

HRESULT CAlphen::Render_LightDepth()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable_LightDepth()))
		return E_FAIL;


	_uint      iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 1)))
			return E_FAIL;

	}

	_uint      iNumFaceMeshContainers = m_pModelFaceCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumFaceMeshContainers; ++i)
	{
		if (FAILED(m_pModelFaceCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelFaceCom->Render(m_pShaderCom, "g_BoneMatrices", i, 1)))
			return E_FAIL;
	}


	_uint      iNumHairMeshContainers = m_pModelHairCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumHairMeshContainers; ++i)
	{
		if (FAILED(m_pModelHairCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelHairCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelHairCom->Render(m_pShaderCom, "g_BoneMatrices", i, 1)))
			return E_FAIL;
	}
	return S_OK;
}


HRESULT CAlphen::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Body"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_ModelFace"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Face"), (CComponent**)&m_pModelFaceCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_ModelHair"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_Hair"), (CComponent**)&m_pModelHairCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_ModelSKL"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Ari_SKL"), (CComponent**)&m_pModelSKLCom)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC         ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.25f, 0.f);
	ColliderDesc.vSize = _float3(0.2f, 0.5, 0.2f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	ColliderDesc.fRadius = 0.25f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_AttackSPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pAttackSphereCom, &ColliderDesc)))
		return E_FAIL;


	// Navi Component 생성
	//CNavigation::NAVIDESC NaviDesc;
	//NaviDesc.iCurrentIndex = 0;
	//NaviDesc.pParent = nullptr;

	//if (FAILED(__super::SetUp_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Map_Tutorial"), (CComponent**)&m_pNaviCom, &NaviDesc)))
	//   return E_FAIL;

	return S_OK;
}

HRESULT CAlphen::SetUp_ConstantTable()
{
	CGameInstance*      pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CAlphen::SetUp_ConstantTable_LightDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_LightTransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_LightTransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	_float fFar = pGameInstance->Get_LightFar();
	if (FAILED(m_pShaderCom->Set_RawValue("g_fLightFar", &fFar, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CAlphen::Key_Input()
{
	CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	_bool flag = false;

	if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
	{
		flag = true;
		m_bMove = true;

		if (pGameInstance->Key_Down(DIK_LSHIFT))
		{
			m_bEvade = true;
		}
	}
	if (pGameInstance->Key_Down(DIK_LSHIFT))
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		pGameInstance->Add_GameObjectToLayer(pGameInstance->Get_LevelIndex(), TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Mesh_Effect"), pGameInstance->Get_MeshEffect_Data(UNIT_BOSS, 1));
		Safe_Release(pGameInstance);
		flag = true;
		m_bEvadeBack = true;
		m_bEvade = true;
	}

	if (pGameInstance->Key_Down(DIK_SPACE))
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		//pGameInstance->Add_GameObjectToLayer(pGameInstance->Get_LevelIndex(), TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Rect_Effect"), pGameInstance->Get_InstanceEffect_Data(UNIT_BOSS, 1));
		Safe_Release(pGameInstance);
		flag = true;
		m_bJump = true;
	}

	if (pGameInstance->Key_Pressing(DIK_E) && m_bSkill == false)
	{
		flag = true;
		m_bSkill = true;
		m_bSkillKeyDown = true;  // 첨에 스킬키 눌리면.
		m_bStartSkill = false;

		if (m_bAir)
		{
			m_eSkillUse = (ARI_SKILL)m_eSkillAirFirst;
		}
		else {
			m_eSkillUse = (ARI_SKILL)m_eSkillGroundFirst;
		}

	}

	if (pGameInstance->Key_Up(DIK_E) && m_bSkill)
	{
		m_bSkillKeyDown = false;
	}

	if (pGameInstance->Key_Pressing(DIK_R) && m_bSkill == false)
	{
		flag = true;
		m_bSkill = true;
		m_bSkillKeyDown = true;  // 첨에 스킬키 눌리면.
		m_bStartSkill = false;
		if (m_bAir)
		{
			m_eSkillUse = (ARI_SKILL)m_eSkillAirSecond;
		}
		else {
			m_eSkillUse = (ARI_SKILL)m_eSkillGroundSecond;
		}
	}

	if (pGameInstance->Key_Up(DIK_R) && m_bSkill)
	{
		m_bSkillKeyDown = false;
	}

	if (pGameInstance->Key_Pressing(DIK_F) && m_bSkill == false)
	{
		flag = true;
		m_bSkill = true;
		m_bSkillKeyDown = true;  // 첨에 스킬키 눌리면.
		m_bStartSkill = false;
		if (m_bAir)
		{
			m_eSkillUse = (ARI_SKILL)m_eSkillAirThird;
		}
		else {
			m_eSkillUse = (ARI_SKILL)m_eSkillGroundThird;
		}
	}

	if (pGameInstance->Key_Up(DIK_F) && m_bSkill)
	{
		m_bSkillKeyDown = false;
	}






	if (m_bSkill == false)
	{
		m_bSkillKeyDown = false;
	}


	m_bKeyInput = flag;


	Safe_Release(pGameInstance);


}

void CAlphen::Key_Input_Town()
{
	CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_bool flag = false;
	if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
	{
		flag = true;
		m_bMove = true;

		if (pGameInstance->Key_Down(DIK_LSHIFT))
		{
			m_bEvade = true;
		}
	}


	if (pGameInstance->Key_Down(DIK_SPACE))
	{
		flag = true;
		m_bJump = true;
	}




	m_bKeyInput = flag;


	Safe_Release(pGameInstance);
}

void CAlphen::Jump(_double TimeDelta)
{
	if (m_bJump)
	{
		m_bLoop = false;
		if (!m_bLand)
		{
			m_bJump = false;
		}
	
		if (m_bAttack || m_bAirAttack)
			m_bJump = false;

		if (m_bAttack == false && m_bSkill == false && m_bEvade == false && m_bEvadeBack == false && m_bAirAttack == false && m_bLand)
		{

			if (m_iAbleJumpCount > 0)
			{
				m_bMove = false;
				m_bAir = true;
				m_bLand = false;

				m_dAnimSpeed = 5.5;
				m_iAbleJumpCount--;
				m_iNextAnimationIndex = ARI_ANIM_BTL_JUMP;

			}
			else {
				//return;
			}
			if (m_iNextAnimationIndex != ARI_ANIM_BTL_JUMP)
			{
				m_bJump = false;
			}

		}
	}

	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_JUMP)
	{
		m_bAir = true;
		_vector  vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float vPosY = XMVectorGetY(vPos);
		m_dJumpSpeed -= TimeDelta*m_dGravityPower*2.f;

		vPosY += (_float)m_dJumpSpeed;
		_vector vUp = XMVectorSet(0.f, (_float)m_dJumpSpeed, 0.f, 1.f);

		// 원본
		if (nullptr == m_pNaviCom) {
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, vPosY));
		}

		// 수정
		else {
			_float3 fPos;
			XMStoreFloat3(&fPos, vPos);


			// Navi 태우기
			if (nullptr != m_pNaviCom) {
				if (m_pNaviCom->Move_OnNavigation(vPos))
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, vPosY, fPos.z, 1.f));
			}
		}



		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
		{
			m_dJumpSpeed = JUMP_SPEED;
			m_iNextAnimationIndex = ARI_ANIM_BTL_FALL;
			m_bJump = false;
		}


	}


	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_JUMP || m_iCurrentAnimationIndex == ARI_ANIM_BTL_FALL)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform"), 0);
		_vector vTargetLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);

		_matrix      RotationMatrix;
		_vector vLook[DIR_END];

		for (_uint i = 0; i < DIR_END; ++i)
		{
			RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f*i));
			vLook[i] = XMVectorSetY(XMVector3TransformNormal(vTargetLook, RotationMatrix), 0.f);
		}



		if (pGameInstance->Key_Pressing(DIK_W))
		{
			if (pGameInstance->Key_Pressing(DIK_D))
			{
				m_ePlayerDir = DIR_WD;

			}
			else if (pGameInstance->Key_Pressing(DIK_A))
			{
				m_ePlayerDir = DIR_AW;

			}
			else {
				m_ePlayerDir = DIR_W;

			}

		}

		else if (pGameInstance->Key_Pressing(DIK_D))
		{
			if (pGameInstance->Key_Pressing(DIK_S))
			{
				m_ePlayerDir = DIR_DS;
			}
			else
			{
				m_ePlayerDir = DIR_D;
			}

		}

		else if (pGameInstance->Key_Pressing(DIK_S))
		{
			if (pGameInstance->Key_Pressing(DIK_A))
			{
				m_ePlayerDir = DIR_SA;
			}
			else
			{
				m_ePlayerDir = DIR_S;
			}
		}
		else if (pGameInstance->Key_Pressing(DIK_A))
		{

			m_ePlayerDir = DIR_A;

		}

		if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
		{
			m_pTransformCom->Go_Straight(TimeDelta*0.5f, m_pNaviCom);
		}
	}






}


void CAlphen::Evade()
{

	if (m_bEvade)
	{
		if (m_bSkill)
		{
			m_bEvade = false;
			return;
		}

		m_bLoop = false;
		m_bMove = false;
		m_bAttack = false;
		m_bJump = false;
		if (m_bSkill == false && m_bAttack == false && m_bAirAttack == false)
		{

			if (m_bAir)
			{
				if (m_iAbleAirEvadeCount > 0)
				{
					m_iAbleAirEvadeCount--;
					m_iNextAnimationIndex = ARI_ANIM_BTL_STEP_AIR;
					m_bJump = false;
					m_bAirAttack = false;
					m_dJumpSpeed = JUMP_SPEED;
					m_dAnimSpeed = 2.7;
				}
				else {
					m_bEvade = false;
				}

			}
			else
			{

				if (m_bEvadeBack)
				{
					m_iNextAnimationIndex = ARI_ANIM_BTL_STEP_LAND_BACK;
					m_dAnimSpeed = 2.5;
				}
				else
				{
					CGameInstance* pGameInstance = CGameInstance::GetInstance();
					CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform"), 0);
					_vector vTargetLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);

					_matrix      RotationMatrix;
					_vector vLook[DIR_END];

					for (_uint i = 0; i < DIR_END; ++i)
					{
						RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f*i));
						vLook[i] = XMVectorSetY(XMVector3TransformNormal(vTargetLook, RotationMatrix), 0.f);
					}
					m_pTransformCom->TurnAxis(vLook[m_ePlayerDir]);
					m_iNextAnimationIndex = ARI_ANIM_BTL_STEP_LAND;
					m_dAnimSpeed = 2.7;
				}

			}

		}
	}

	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_STEP_AIR
		|| m_iCurrentAnimationIndex == ARI_ANIM_BTL_STEP_LAND_BACK
		|| m_iCurrentAnimationIndex == ARI_ANIM_BTL_STEP_LAND)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 14)
		{
			m_bMove = false;
			m_bEvade = false;
			m_bEvadeBack = false;
			m_iLinearTime = 0.5;
		}
	}

	if (!m_bEvade)
	{
		m_bEvadeBack = false;
	}








}

void CAlphen::Attack(_double TimeDelta)
{

	CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Button_Pressing(CInput_Device::DIMB_LBUTTON))
	{
		if (!m_bEvade && !m_bSkill)
		{
			m_bAttackKeyInput = true;

		}
	}
	if (pGameInstance->Button_Up(CInput_Device::DIMB_LBUTTON)) {
		m_bAttackKeyInput = false;
	}

	if (m_bAttack || m_bAirAttack)
	{
		m_bMove = false;
	}

	if (m_bAttackKeyInput)
	{
		if (!m_bAir&&m_bLand)
		{
			m_bLoop = false;
			m_dAnimSpeed = 1.75;
			m_iLinearTime = 0.5;
			switch (m_iCurrentAnimationIndex)
			{

			case ARI_ANIM_BTL_ATTACK_NORMAL_0:
				m_iWaitAnimationIndex = ARI_ANIM_BTL_ATTACK_NORMAL_1;
				break;
			case ARI_ANIM_BTL_ATTACK_NORMAL_1:
				m_iWaitAnimationIndex = ARI_ANIM_BTL_ATTACK_NORMAL_8;
				break;
			case ARI_ANIM_BTL_ATTACK_NORMAL_8:
				m_iWaitAnimationIndex = ARI_ANIM_BTL_ATTACK_NORMAL_6;
				break;
			case ARI_ANIM_BTL_ATTACK_NORMAL_6:
				break;
			default:
				m_iWaitAnimationIndex = ARI_ANIM_BTL_ATTACK_NORMAL_0;
				break;

			}


			if (m_iWaitAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_0)
			{

				m_iNextAnimationIndex = m_iWaitAnimationIndex;
				if (m_pVecMonsters->size() > 0)
				{
					m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
				}
				m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
				m_AttackDamageInfo.fPowerBack = 0.010f;
				m_AttackDamageInfo.fPowerUp = 0.005f;
				m_bAttack = true;
			}

			else if (m_iWaitAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_1&&m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_0)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 19)
				{
					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					if (m_pVecMonsters->size() > 0)
					{
						m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
					}
					m_bAttack = true;
					m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
					m_AttackDamageInfo.fPowerBack = 0.010f;
					m_AttackDamageInfo.fPowerUp = 0.005f;


				}
			}

			else if (m_iWaitAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_8&&m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_1)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 29)
				{

					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					if (m_pVecMonsters->size() > 0)
					{
						m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
					}
					m_bAttack = true;

					m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
					m_AttackDamageInfo.fPowerBack = 0.010f;
					m_AttackDamageInfo.fPowerUp = 0.005f;

				}
			}

			else if (m_iWaitAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_6&&m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_8)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 29)
				{
					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					if (m_pVecMonsters->size() > 0)
					{
						m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
					}
					m_bAttack = true;
					m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
					m_AttackDamageInfo.fPowerBack = 0.010f;
					m_AttackDamageInfo.fPowerUp = 0.016f;

					m_iWaitAnimationIndex = ARI_ANIM_END;
				}
			}

		}

		else if (!m_bLand&&m_bAir)
		{
			m_bJump = false;
			m_bLoop = false;
			m_dAnimSpeed = 1.8;
			m_iLinearTime = 0.5;
			switch (m_iCurrentAnimationIndex)
			{

			case ARI_ANIM_BTL_ATTACK_NORMAL_AIR_0:
				m_iWaitAnimationIndex = ARI_ANIM_BTL_ATTACK_NORMAL_AIR_1;
				break;
			case ARI_ANIM_BTL_ATTACK_NORMAL_AIR_1:
				m_iWaitAnimationIndex = ARI_ANIM_BTL_ATTACK_NORMAL_AIR_2;
				break;
			case ARI_ANIM_BTL_ATTACK_NORMAL_AIR_2:
				break;
			default:
				m_iWaitAnimationIndex = ARI_ANIM_BTL_ATTACK_NORMAL_AIR_0;
				break;

			}
			if (m_iAbleAirAttackCount > 0)
			{
				if (m_iWaitAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_0)
				{

					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					m_bAirAttack = true;
					m_iAbleAirAttackCount--;


					m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
					m_AttackDamageInfo.fPowerBack = 0.016f;
					m_AttackDamageInfo.fPowerUp = 0.013f;
				}
			}
			else {
				m_bAirAttack = false;
			}



			if (m_iWaitAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_1&&m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_0)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 30)
				{
					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					if (m_pVecMonsters->size() > 0)
					{
						m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
					}
					m_bAirAttack = true;

					m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
					m_AttackDamageInfo.fPowerBack = 0.016f;
					m_AttackDamageInfo.fPowerUp = 0.013f;

				}
			}

			else if (m_iWaitAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_2&&m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_1)
			{
				if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 17)
				{

					m_iNextAnimationIndex = m_iWaitAnimationIndex;
					if (m_pVecMonsters->size() > 0)
					{
						m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
					}
					m_bAirAttack = true;
					m_iWaitAnimationIndex = ARI_ANIM_END;

					m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
					m_AttackDamageInfo.fPowerBack = 0.016f;
					m_AttackDamageInfo.fPowerUp = 0.013f;

				}
			}

		}


	}

	if (m_bAttack)
	{
		if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_0&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 72 ||
			m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_1&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 75 ||
			m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_8&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 130 ||
			m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_6&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 105)
		{
			m_bAttack = false;
		}

	}
	if (m_bAirAttack)
	{
		m_bEvade = false;
		if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_0&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 47 ||
			m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_1&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 33 ||
			m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_2&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 47)
		{
			m_bAirAttack = false;
		}

	}

	//콜라이더 온오프
	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_0)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 20 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 17)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}
	}

	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_1)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 5 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 9)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 21 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 24)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}
		else {
			m_bOnAttackCollider = false;
		}
	}

	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_8)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 20 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 24)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}
	}

	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_6)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 13 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 17)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}
	}

	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_0)
	{
		//m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
		//m_AttackDamageInfo.fPowerBack = 0.01f;
		//m_AttackDamageInfo.fPowerUp = 0.01f;

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 17 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 21)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f + XMVectorSet(0.f, 0.5f, 0.f, 1.f), 0.6f);
		}
	}


	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_1)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 7 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 10)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f + XMVectorSet(0.f, 0.5f, 0.f, 1.f), 0.6f);
		}
	}


	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NORMAL_AIR_2)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 7 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 11)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f + XMVectorSet(0.f, 0.5f, 0.f, 1.f), 0.6f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 27 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 32)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f + XMVectorSet(0.f, 0.5f, 0.f, 1.f), 0.6f);
		}
		else {
			m_bOnAttackCollider = false;
		}
	}




	Safe_Release(pGameInstance);
}

void CAlphen::Skill(_double TimeDelta)
{
	if (m_bSkill)
	{
		m_bLoop = false;


		switch (m_eSkillUse)
		{
		case Client::CAlphen::SKILL_LIGTHING_THRUST:
			Lightning_Thrust();
			break;
		case Client::CAlphen::SKILL_SWALLOW_BLADE:
			Swallow_Balde();
			break;
		case Client::CAlphen::SKILL_DRAGON_SWARM:
			Dragon_Swarm();
			break;
		case Client::CAlphen::SKILL_VOID_FLASH:
			break;
		case Client::CAlphen::SKILL_RISING_PALCON:
			Rising_Palcon();
			break;
		case Client::CAlphen::SKILL_HURRICANE_THRUST:
			Hurricane_Thrust(TimeDelta);
			break;
		case Client::CAlphen::SKILL_END:
			break;
		default:
			break;
		}

	}


}




void CAlphen::Compute_Anim(_double TimeDelta)
{
	CGameInstance*      pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	if (m_bMove && !m_bSkill && !m_bEvade && !m_bAttack && !m_bSkill && !m_bJump &&m_bLand && !m_bAir)
	{
		Compute_Move(TimeDelta);
	}


	Jump(TimeDelta);
	Skill(TimeDelta);
	Evade();
	if (m_bBattle)
	{
		Attack(TimeDelta);

	}











	if (m_bAir)
	{
		m_bMove = false;
		m_bAttack = false;
	}
	else {
		m_bAirAttack = false;
		m_iAbleJumpCount = 1;
		m_iAbleAirEvadeCount = 1;
		m_iAbleAirAttackCount = 1;
		m_dFallSpeed = 0.0;
		m_dJumpSpeed = JUMP_SPEED;
	}
	if (m_bKeyInput == false)
	{
		m_bMove = false;
	}
	if (
		m_bAttack == false &&
		m_bEvade == false &&
		m_bSkill == false &&
		m_bJump == false &&
		m_bMove == false &&
		m_bAirAttack == false &&
		m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
	{
		if (!m_bAir)
		{
			m_bLoop = true;

			m_iNextAnimationIndex = ARI_ANIM_BTL_MOVE_IDLE;
		}
		else {
			m_bLoop = true;
			m_iNextAnimationIndex = ARI_ANIM_BTL_FALL;
		}
	}


	_vector  vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float vPosY = XMVectorGetY(vPos);

	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_FALL)
	{
		//m_bMove = false;
		m_dFallSpeed += TimeDelta*m_dGravityPower*2.f;
		m_bLand = false;

		vPosY -= (_float)m_dFallSpeed;

		// 네비메쉬가 존재할 경우
		if (nullptr != m_pNaviCom) {
			// 현재 위치에서 네비 메쉬의 높이를 가져온다.
			_float fHeight = m_pTransformCom->Get_Height(m_pNaviCom);

			// 만약 떨어지고 난 다음의 높이가 네비 메쉬의 y값보다 더 작다면
			if (vPosY < fHeight) {
				// 현재 위치의 y값을 네비 메쉬의 y값으로 수정하고, 이후 애니메이션을 착지 애니메이션으로, 점프 관련 변수를 초기화 한다.
				m_bLoop = false;
				vPosY = fHeight;
				m_dFallSpeed = 0.0;
				m_iNextAnimationIndex = ARI_ANIM_BTL_LAND;
			}
		}
		else {
			// 현재 위치에서 네비 메쉬의 높이를 가져온다.
			_float fHeight = m_dCurrBattleMap_Height;

			// 만약 떨어지고 난 다음의 높이가 네비 메쉬의 y값보다 더 작다면
			if (vPosY < fHeight) {
				// 현재 위치의 y값을 네비 메쉬의 y값으로 수정하고, 이후 애니메이션을 착지 애니메이션으로, 점프 관련 변수를 초기화 한다.
				m_bLoop = false;
				vPosY = fHeight;
				m_dFallSpeed = 0.0;
				m_iNextAnimationIndex = ARI_ANIM_BTL_LAND;
			}
		}

		vPos = XMVectorSetY(vPos, vPosY);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, vPosY));
	}

	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_LAND&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 15)
	{
		m_bLand = true;
	}



	//if (m_bSkill)
	//{
	//   m_bLoop = false;
	//   Skill();
	//}


	if (nullptr != m_pNaviCom) {
		if (vPosY <= m_pTransformCom->Get_Height(m_pNaviCom))
		{
			m_bAir = false;

		}
	}
	else {
		if (vPosY <= m_dCurrBattleMap_Height)
		{
			m_bAir = false;

		}
	}



	Safe_Release(pGameInstance);



}

void CAlphen::Compute_Move(_double TimeDelta)
{


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform"), 0);
	_vector vTargetLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);

	_matrix      RotationMatrix;
	_vector vLook[DIR_END];

	for (_uint i = 0; i < DIR_END; ++i)
	{
		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f*i));
		vLook[i] = XMVectorSetY(XMVector3TransformNormal(vTargetLook, RotationMatrix), 0.f);
	}







	//

	m_bLoop = true;
	m_iLinearTime = 0.0;

	m_iNextAnimationIndex = ARI_ANIM_BTL_MOVE_RUN;
	m_dAnimSpeed = 1.85;


	if (pGameInstance->Key_Pressing(DIK_W))
	{
		if (pGameInstance->Key_Pressing(DIK_D))
		{

			m_ePlayerDir = DIR_WD;

		}
		else if (pGameInstance->Key_Pressing(DIK_A))
		{
			m_ePlayerDir = DIR_AW;

		}
		else {
			m_ePlayerDir = DIR_W;

		}

	}

	else if (pGameInstance->Key_Pressing(DIK_D))
	{
		if (pGameInstance->Key_Pressing(DIK_S))
		{
			m_ePlayerDir = DIR_DS;
		}
		else
		{
			m_ePlayerDir = DIR_D;
		}

	}

	else if (pGameInstance->Key_Pressing(DIK_S))
	{
		if (pGameInstance->Key_Pressing(DIK_A))
		{
			m_ePlayerDir = DIR_SA;
		}
		else
		{
			m_ePlayerDir = DIR_S;
		}
	}
	else if (pGameInstance->Key_Pressing(DIK_A))
	{

		m_ePlayerDir = DIR_A;

	}
	else
	{
		m_iNextAnimationIndex = ARI_ANIM_BTL_MOVE_BRAKE;
		m_bLoop = false;
	}



	//카메라 기준 왼쪽키 누르면 카메라 룩하고  내 룩하고 90도 -90? 될떄까지 턴 . 
	//카메라 기준 오른쪾키 누르면 90 




	_vector vPlayerLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vGoalLook = vLook[m_ePlayerDir];
	//_float  fDot = XMVectorGetX(XMVector3Dot(vPlayerLook, vGoalLook)); // 현재 내룩과 키입력 받은 룩을 내적 
	//내적값이 -10~10이면 일치, -면 왼쪽 +면 오른쪽 
	//내적값은 라디안인가..?

	//방향이 일치하는지부터 true false로 반환하게끔


	//



	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_MOVE_RUN)
	{

		m_pTransformCom->Check_Right_Left(vGoalLook, TimeDelta*6.5f);


		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook = vLook * (_float)TimeDelta * 5.f;

		// Navi 태우기
		m_pTransformCom->Go_Straight(TimeDelta*0.5f, m_pNaviCom);

		// y값 조정
		if (nullptr != m_pNaviCom) {
			m_pTransformCom->Set_Height(m_pTransformCom->Get_Height(m_pNaviCom));
		}
		else{
			m_pTransformCom->Set_Height(m_dCurrBattleMap_Height);
		}
	}


	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_MOVE_BRAKE&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 5)
	{
		m_bLoop = false;
		m_bMove = false;

	}





	Safe_Release(pGameInstance);
}

void CAlphen::Rising_Palcon()
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);


	if (!m_bStartSkill)
	{
		m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_HITENSYOUKU_START;
		m_bStartSkill = true;
		m_bLoop = false;
		m_bJump = false;
		m_dAnimSpeed = 1.75;

	}


	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_HITENSYOUKU_START
		&&m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
	{
		m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_HITENSYOUKU_LOOP;

	}

	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_HITENSYOUKU_LOOP)
	{
		m_pAttackSphereCom->Set(vPos + vLook*0.4f, 0.45f);
		m_bOnAttackCollider = true;
		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
		{
			m_bOnAttackCollider = false;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
			m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_HITENSYOUKU_END;

			m_bLand = true;
			m_bAir = false;
		}

		else {
			m_pModelCom->Get_CurAnimation()->Reset_Animation();
			m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
			m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
			m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
		}
	}
	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_HITENSYOUKU_END)
	{
		m_bOnAttackCollider = true;
		m_pAttackSphereCom->Set(vPos + vLook, 1.8f); //어택스피어 콜라이더 크기바꿈
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 20)
		{
			m_bOnAttackCollider = false;
		}
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 85)
		{

			if (m_bSkillKeyDown == true)
			{
				m_iNextAnimationIndex = 33;
			}
			else {
				m_bSkill = false;
				m_bStartSkill = false;
			}
		}
	}

	Infernal_Torrent();

	//else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_MAOUENGEKIHA_START2&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
	//{
	//   m_iNextAnimationIndex = 32;
	//}

	//else if (m_iCurrentAnimationIndex == 32 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 20)
	//{

	//   m_iNextAnimationIndex = 31;
	//}

	//else if (m_iCurrentAnimationIndex == 31
	//   && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
	//{
	//   m_bSkill = false;
	//   m_bStartSkill = false;
	//}

}

void CAlphen::Hurricane_Thrust(_double TimeDelta)
{


	if (!m_bStartSkill)
	{
		m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_HUZINKEN;
		if (m_pVecMonsters->size() > 0)
		{
			m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
		}

		m_bStartSkill = true;
		m_bLoop = false;
		m_bJump = false;
		m_dAnimSpeed = 2.4;

		m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_LARGE;
		m_AttackDamageInfo.fPowerBack = 0.026f;
		m_AttackDamageInfo.fPowerUp = 0.02f;

	}




	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_HUZINKEN)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 70)
		{

			if (m_bSkillKeyDown == true)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_NEPPASENPUUZIN_START;
				m_dAnimSpeed = 2.4;
			}
			else {
				m_bSkill = false;
				m_bStartSkill = false;
				m_dAnimSpeed = 2.3;
				m_bPattern = false;
			}
		}
	}

	Searing_Gale(TimeDelta);




	if (m_iCurrentAnimationIndex == 26)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 29 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 36)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.6f + XMVectorSet(0.f, 0.5f, 0.f, 1.f), 0.7f);
		}
	}


}

void CAlphen::Infernal_Torrent()
{
	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_MAOUENGEKIHA_START2)
	{
		if (!m_bTorrent)
		{
			m_bTorrent = true;
			m_pNormalSword->Set_SocketMatrix(m_NormalBone);
			m_pFlamSword->Set_SocketMatrix(m_HandBone);
		}
	}


	if (m_iCurrentAnimationIndex == 33
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
	{

		m_iNextAnimationIndex = 32;
	}

	if (m_iCurrentAnimationIndex == 32)
	{
		if (m_bSkillKeyDown)
		{
			m_tPlayerInfo.m_iCurrentHp--;
	
			if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
			{

				m_iNextAnimationIndex = 31;
			}
		}
		else {
			m_iNextAnimationIndex = 31;
		}

	}

	if (m_iCurrentAnimationIndex == 31
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 90)
	{
		m_pNormalSword->Set_SocketMatrix(m_HandBone);
		m_pFlamSword->Set_SocketMatrix(m_FlamBone);
		m_bSkill = false;
		m_bStartSkill = false;
		m_bTorrent = false;
		m_bPattern = false;
	}
}

void CAlphen::Searing_Gale(_double TimeDelta)
{
	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NEPPASENPUUZIN_START)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 34)
		{
			m_iNextAnimationIndex = 40;

		}

		if (!m_bGale)
		{
			m_bGale = true;
			m_pNormalSword->Set_SocketMatrix(m_NormalBone);
			m_pFlamSword->Set_SocketMatrix(m_HandBone);
		}

	}


	if (m_iCurrentAnimationIndex == 40)
	{
		if (m_bSkillKeyDown)
		{
			m_tPlayerInfo.m_iCurrentHp--;
			if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
			{

				m_iNextAnimationIndex = 39;
			}
		}
		else {
			m_iNextAnimationIndex = 39;
		}

	}


	if (m_iCurrentAnimationIndex == 39)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
		{

			m_iNextAnimationIndex = 37;
			m_dAnimSpeed = 10.0;
		}
	}


	else if (m_iCurrentAnimationIndex == 37)
	{

		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
		{

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
			m_iNextAnimationIndex = 36;
			m_dAnimSpeed = 2.4;
		}
		else {
			m_pTransformCom->Go_Down(TimeDelta*2.f);
			if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
			{
				m_dAnimSpeed = 10.0;
				m_pModelCom->Get_CurAnimation()->Reset_Animation();
				m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
				m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
				m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
			}
		}

	}


	else if (m_iCurrentAnimationIndex == 36)
	{

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 150)
		{
			m_dAnimSpeed = 2.1;
			m_pNormalSword->Set_SocketMatrix(m_HandBone);
			m_pFlamSword->Set_SocketMatrix(m_FlamBone);
			m_bSkill = false;
			m_bStartSkill = false;
			m_bAir = false;
			m_bJump = false;
			m_bGale = false;
			m_bLand = true;
			m_bPattern = false;
		}

	}



	//콜라이더
	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_NEPPASENPUUZIN_START)
	{
		m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
		m_AttackDamageInfo.fPowerBack = 0.002f;
		m_AttackDamageInfo.fPowerUp = 0.020f;

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 2 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 4)
		{

			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.45f);
		}

	}

	else if (m_iCurrentAnimationIndex == 37)
	{
		m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
		m_AttackDamageInfo.fPowerBack = 0.002f;
		m_AttackDamageInfo.fPowerUp = 0.015f;
		m_bOnAttackCollider = true;
		m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.45f);

	}

	else if (m_iCurrentAnimationIndex == 36)
	{


		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 2 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 4)
		{
			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.002f;
			m_AttackDamageInfo.fPowerUp = 0.013f;

			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.45f);
		}



		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 40 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 42)
		{

			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.003f;
			m_AttackDamageInfo.fPowerUp = 0.017f;
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.7f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.6f);
		}


		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 52 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 54)
		{

			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_LARGE;
			m_AttackDamageInfo.fPowerBack = 0.004f;
			m_AttackDamageInfo.fPowerUp = 0.022f;
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*1.f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.6f);
		}

	}


















}

void CAlphen::Risiing_Phoenix()
{

	if (m_iCurrentAnimationIndex == 23)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 50)
		{
			m_iNextAnimationIndex = 22;
			m_bOverlap = true;

		}

		if (!m_bPhoenix)
		{
			m_bPhoenix = true;
			m_pNormalSword->Set_SocketMatrix(m_NormalBone);
			m_pFlamSword->Set_SocketMatrix(m_HandBone);
		}

	}
	else if (m_iCurrentAnimationIndex == 22)
	{
		m_dAnimSpeed = 9.0;
		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
			m_iNextAnimationIndex = 21;
			//m_bOverlap = false;
			m_dAnimSpeed = 2.1;
		}

		else {
			if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
			{
				m_pModelCom->Get_CurAnimation()->Reset_Animation();
				m_pModelHairCom->Get_CurAnimation()->Reset_Animation();
				m_pModelFaceCom->Get_CurAnimation()->Reset_Animation();
				m_pModelSKLCom->Get_CurAnimation()->Reset_Animation();
			}
		}

	}

	else if (m_iCurrentAnimationIndex == 21)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 59)
		{
			m_bOverlap = false;
			m_pNormalSword->Set_SocketMatrix(m_HandBone);
			m_pFlamSword->Set_SocketMatrix(m_FlamBone);
			m_bSkill = false;
			m_bStartSkill = false;
			m_bAir = false;
			m_bJump = false;
			m_bPhoenix = false;
			m_bLand = true;
			m_bPattern = false;
		}

	}



	if (m_iCurrentAnimationIndex == 22)
	{


		m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
		m_AttackDamageInfo.fPowerBack = 0.002f;
		m_AttackDamageInfo.fPowerUp = 0.013f;

		m_bOnAttackCollider = true;
		m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.1f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.45f);


	}

	else if (m_iCurrentAnimationIndex == 21)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 1 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 3)
		{

			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.003f;
			m_AttackDamageInfo.fPowerUp = 0.017f;
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.1f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.6f);
		}


		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 25 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 27)
		{

			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_LARGE;
			m_AttackDamageInfo.fPowerBack = 0.004f;
			m_AttackDamageInfo.fPowerUp = 0.025f;
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*-0.4f + XMVectorSet(0.f, 0.3f, 0.f, 1.f), 0.6f);
		}

	}





}

void CAlphen::Mirage()
{
	if (!m_bStartSkill)
	{
		m_iNextAnimationIndex = 29;
		m_bStartSkill = true;
		m_bLoop = false;

	}


	if (m_iCurrentAnimationIndex == 29
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
	{
		m_iNextAnimationIndex = 28;
	}

	if (m_iCurrentAnimationIndex == 28 && XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= 0.f)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f));
		m_iNextAnimationIndex = 27;
	}

	if (m_iCurrentAnimationIndex == 27
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
	{
		m_bSkill = false;
		m_bStartSkill = false;
		m_bPattern = false;
	}
}

void CAlphen::Rending_Flash()
{
	if (!m_bStartSkill)
	{
		m_iNextAnimationIndex = 58;
		m_bStartSkill = true;
		m_bLoop = false;

	}


	if (m_iCurrentAnimationIndex == 58
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
	{
		m_iNextAnimationIndex = 57;
	}

	if (m_iCurrentAnimationIndex == 57 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 20)
	{

		m_iNextAnimationIndex = 56;
	}

	if (m_iCurrentAnimationIndex == 56
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
	{
		m_bSkill = false;
		m_bStartSkill = false;
		m_bPattern = false;
	}
}

void CAlphen::Explosive_Ring()
{
	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_START)
	{
		if (!m_bRing) {
			m_bRing = true;
			m_pNormalSword->Set_SocketMatrix(m_NormalBone);
			m_pFlamSword->Set_SocketMatrix(m_HandBone);
		}
	}


	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_START
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2)
	{

		m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_LOOP;
	}

	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_LOOP)
	{
		if (m_bSkillKeyDown)
		{
			m_tPlayerInfo.m_iCurrentHp--;
			if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
			{

				m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_END;
			}
		}
		else {
			m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_END;
		}

	}

	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_END
		&& m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 233)
	{
		m_pNormalSword->Set_SocketMatrix(m_HandBone);
		m_pFlamSword->Set_SocketMatrix(m_FlamBone);
		m_bSkill = false;
		m_bStartSkill = false;
		m_bRing = false;
		m_bPattern = false;
	}



	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_END)
	{
		m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
		m_AttackDamageInfo.fPowerBack = 0.002f;
		m_AttackDamageInfo.fPowerUp = 0.f;

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 34 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 36)
		{

			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 50 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 52)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 62 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 64)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 72 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 74)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}
		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 97 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 99)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}
		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 113 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 115)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 122 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 124)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 135 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 137)
		{
			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_DOWN;
			m_AttackDamageInfo.fPowerBack = 0.03f;
			m_AttackDamageInfo.fPowerUp = 0.02f;
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 0.45f);
		}


	}

}

void CAlphen::Lightning_Thrust()
{


	if (m_bEvade == false && m_bJump == false && m_bLand)
	{
		m_bMove = false;
		m_bAttack = false;


		if (!m_bStartSkill)
		{
			m_iNextAnimationIndex = 65;

			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.028f;
			m_AttackDamageInfo.fPowerUp = 0.025f;
			if (m_pVecMonsters->size() > 0)
			{
				m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
			}
			m_dAnimSpeed = 2.4;
			m_bStartSkill = true;
			m_bLoop = false;

		}
	}




	if (m_iCurrentAnimationIndex == 65)
	{
		m_bAir = true;
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 58)
		{
			if (m_bSkillKeyDown == true)
			{
				m_iNextAnimationIndex = 23;
				if (m_pVecMonsters->size() > 0)
				{
					m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
				}
				m_dAnimSpeed = 2.4;

			}
			else {
				m_iNextAnimationIndex = ARI_ANIM_BTL_FALL;
				m_dAnimSpeed = 2.1;
				m_bSkill = false;
				m_bPattern = false;
				m_bStartSkill = false;
			}


		}
	}

	Risiing_Phoenix();

	if (m_iCurrentAnimationIndex == 65)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 19 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 21)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.2f, 1.f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 23 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 25)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.2f, 1.f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 29 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 31)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.2f, 1.f);
		}
	}
}

void CAlphen::Swallow_Balde()
{

	if (m_bEvade == false && m_bJump == false)
	{
		m_bMove = false;
		m_bAttack = false;


		if (!m_bStartSkill)
		{
			m_iNextAnimationIndex = 17;
			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.0005f;
			m_AttackDamageInfo.fPowerUp = 0.0005f;
			if (m_pVecMonsters->size() > 0)
			{
				m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
			}
			m_bStartSkill = true;
			m_bLoop = false;

		}
	}




	if (m_iCurrentAnimationIndex == 17)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 85)
		{
			if (m_bSkillKeyDown == true)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_MAOUENGEKIHA_START2;
				m_dAnimSpeed = 2.1;
				if (m_pVecMonsters->size() > 0)
				{
					m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
				}
			}
			else {
				m_bSkill = false;
				m_bPattern = false;
				m_bStartSkill = false;
			}


		}
	}

	Infernal_Torrent();

	if (m_iCurrentAnimationIndex == 17)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 11 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 13)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 1.f);
		}

		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 21 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 23)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.4f, 1.f);
		}

	}
}

void CAlphen::Dragon_Swarm()
{
	if (m_bEvade == false && m_bJump == false)
	{
		m_bMove = false;
		m_bAttack = false;


		if (!m_bStartSkill)
		{
			m_iNextAnimationIndex = 64;
			if (m_pVecMonsters->size() > 0)
			{
				m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
			}
			m_bStartSkill = true;
			m_bLoop = false;

		}
	}




	if (m_iCurrentAnimationIndex == 64)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 90)
		{
			if (m_bSkillKeyDown == true)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_ATTACK_GOUKABAKUENNZIN_START;
				if (m_pVecMonsters->size() > 0)
				{
					m_pTransformCom->TurnAxis(XMVectorSetY((*m_iter)->Get_EnemyPos(), 0.f) - XMVectorSetY(Get_PlayerPos(), 0.f));
				}
			}
			else {
				m_bSkill = false;
				m_bPattern = false;
				m_bStartSkill = false;
			}


		}
	}


	Explosive_Ring();



	//콜라이더
	if (m_iCurrentAnimationIndex == 64)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 22 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 24)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.2f, 1.f);

			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.014f;
			m_AttackDamageInfo.fPowerUp = 0.003f;
		}
		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 48 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 50)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.2f, 1.f);
			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.014f;
			m_AttackDamageInfo.fPowerUp = 0.003f;
		}
		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 70 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 72)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.2f, 1.f);
			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.014f;
			m_AttackDamageInfo.fPowerUp = 0.003f;
		}
		else if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 80 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 82)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(Get_PlayerPos() + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.2f, 1.f);
			m_AttackDamageInfo.e_DamageType = DAMAGE_TYPE_SMALL;
			m_AttackDamageInfo.fPowerBack = 0.014f;
			m_AttackDamageInfo.fPowerUp = 0.003f;
		}
	}
	/*

	22 24
	48 50
	70 72
	80 82*/

}

void CAlphen::Compute_Look()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
}

void CAlphen::Compute_Hit(_double TimeDelta)
{




	if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
	{
		m_bHit = false;
		m_bDamage_Small_b = false;


		//모든 변수 초기화?
		m_bInvincible = false;
		m_bAttack = false;
		m_bAirAttack = false;
		m_bSkill = false;
		m_bJump = false;
		m_bEvade = false;
		m_bEvadeBack = false;


		if (m_bAir)
		{
			m_iNextAnimationIndex = ARI_ANIM_BTL_FALL;
		}
		else {
			m_iNextAnimationIndex = ARI_ANIM_BTL_MOVE_IDLE;
			m_bLand = true; //위로 올려야 할수도?
		}

	}


}

void CAlphen::Compute_Attack()
{







	m_pAttackSphereCom->Set_On(m_bOnAttackCollider);
}

void CAlphen::Compute_HitInfo()
{

	switch (m_HitDamageInfo.e_DamageType)
	{
	case Client::DAMAGE_TYPE_DOWN:
		if (m_HitDamageInfo.e_DamageDir == DAMAGE_DIR_FRONT)
		{
			if (m_bAir)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_BLOW_UP_F;
			}
			else {
				m_iNextAnimationIndex = ARI_ANIM_BTL_DOWN_F;
			}

		}
		else {
			if (m_bAir)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_BLOW_UP_B;
			}
			else {
				m_iNextAnimationIndex = ARI_ANIM_BTL_DOWN_B;
			}
		}
		break;
	case Client::DAMAGE_TYPE_LARGE:
		if (m_HitDamageInfo.e_DamageDir == DAMAGE_DIR_FRONT)
		{
			if (m_bAir)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_AIR_LARGE_F;
			}
			else {
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_LARGE_F;
			}

		}
		else {
			if (m_bAir)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_AIR_LARGE_B;
			}
			else {
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_LARGE_B;
			}
		}


		break;
	case Client::DAMAGE_TYPE_SMALL:
		if (m_HitDamageInfo.e_DamageDir == DAMAGE_DIR_FRONT)
		{
			if (m_bAir)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_AIR_SMALL_F;
			}
			else {
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_SMALL_F;
			}

		}
		else {
			if (m_bAir)
			{
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_AIR_SMALL_B;
			}
			else {
				m_iNextAnimationIndex = ARI_ANIM_BTL_DAMAGE_SMALL_B;
			}
		}
		break;
	}

	m_bLoop = false;
	m_bInvincible = true;




}

void CAlphen::Pattern_Choice()
{
	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_MOVE_IDLE)
	{
		m_iNextAnimationIndex = ARI_ANIM_BTL_MOVE_BRAKE;
		return;
	}

	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_MOVE_BRAKE)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() < m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
		{
			return;
		}
	}

	_vector vTargetPos, vDir;

	_float fDistance = 0.f;

	m_bAIGroundSkillStart = false;
	if (m_pVecMonsters->size() > 0)
	{
		vTargetPos = (*m_iter)->Get_EnemyPos();

		fDistance = XMVectorGetX(XMVector3Length(Get_PlayerPos() - vTargetPos));  //내 타깃과의 거리

		vDir = XMVector3Normalize(vTargetPos - Get_PlayerPos());    //내와 타깃사이의 방향벡터
	}


	//공중에있을때
	if (m_bAir)
	{
		m_iNextAnimationIndex = ARI_ANIM_BTL_FALL;
		m_ePattern = ARI_AI_FALL;
		m_bPattern = true;
		return;

	}
	else {
		m_dJumpSpeed = 0.03;
		m_iAbleJumpCount = 1;
		m_bAIJumpStart = false;
	}


	//일단 힐은 넘기고

	//전투관련 거리에따라 행동을 나누자.


	if (fDistance > 3.5f)  //일정거리 이상 멀어진다면 타깃을 쫒아간다.
	{

		m_ePattern = ARI_AI_CHASE;
		m_bPattern = true;
		return;
	}

	//else if (fDistance < 1.f)
	//{
	//	m_ePattern = ARI_AI_EVADE;     //일정거리 미만이면 도망~ 런 
	//	m_bPattern = true;
	//}

	else

	{
		_uint iPatternRate = rand() % 100 + 1;

		if (iPatternRate > 40)
		{
			m_ePattern = ARI_AI_GROUNDBATTLE;
			if (iPatternRate > 50)
			{
				m_bSkillKeyDown = false;

			}
			else {
				m_bSkillKeyDown = true;
			}
		}
		else if (iPatternRate > 30)
		{
			m_ePattern = ARI_AI_MOVE;
		}
		else {
			m_ePattern = ARI_AI_AIRBATTLE;
		}

		m_bPattern = true;
		return;
	}
}

void CAlphen::AirBattle(_double TimeDelta)
{
	if (!m_bAIJumpStart)
	{
		m_bJump = true;
		m_bAIJumpStart = true;
	}
	if (m_bJump)
	{
		Jump(TimeDelta);
	}


	if (!m_bJump)
	{
		m_bSkill = true;
		_uint i = rand() % 3;
		switch (i)
		{

		case 0:
			m_eSkillUse = SKILL_RISING_PALCON;
			break;
		case 1:
			m_eSkillUse = SKILL_RISING_PALCON;
			break;
		case 2:
			m_eSkillUse = SKILL_HURRICANE_THRUST;
			break;
		}

	}

}

void CAlphen::GroundBattle(_double TimeDelta)
{

	if (!m_bAIGroundSkillStart)
	{

		m_bAIGroundSkillStart = true;
		m_bSkill = true;
		_uint i = rand() % 3;
		switch (i)
		{
		case 0:
			m_eSkillUse = SKILL_LIGTHING_THRUST;
			break;
		case 1:
			m_eSkillUse = SKILL_SWALLOW_BLADE;
			break;
		case 2:
			m_eSkillUse = SKILL_DRAGON_SWARM;
			break;
		}


	}

}

void CAlphen::Move(_double TimeDelta)
{
	_vector vTargetPos, vDir, vLook;

	_float fDistance = 0.f;

	m_MoveTime += TimeDelta;
	//몬스터를 향해 일정범위까지간다.
	//회피를 한다 ?
	// 돌아다닌다.

	if (m_pVecMonsters->size() > 0)
	{
		vTargetPos = (*m_iter)->Get_EnemyPos();

		fDistance = XMVectorGetX(XMVector3Length(Get_PlayerPos() - vTargetPos));  //내 타깃과의 거리

		vDir = XMVector3Normalize(vTargetPos - Get_PlayerPos());    //내와 타깃사이의 방향벡터

		vLook = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 1.f), vDir);

	}









	m_bSkill = false;
	m_bAttack = false;
	m_iNextAnimationIndex = ARI_ANIM_BTL_MOVE_RUN;
	m_bLoop = true;


	if (m_pVecMonsters->size() > 0)
	{


		m_pTransformCom->Go_Straight(TimeDelta*0.4f);
		m_pTransformCom->Check_Right_Left(vLook, TimeDelta*0.6f);



	}


	if (m_MoveTime > 0.2)
	{
		m_bPattern = false;
		m_MoveTime = 0.0;

	}



}

void CAlphen::Chase(_double TimeDelta)
{


	_vector vTargetPos, vDir, vLook;

	_float fDistance = 0.f;


	if (m_pVecMonsters->size() > 0)
	{
		vTargetPos = (*m_iter)->Get_EnemyPos();

		fDistance = XMVectorGetX(XMVector3Length(Get_PlayerPos() - vTargetPos));  //내 타깃과의 거리

		vDir = XMVector3Normalize(vTargetPos - Get_PlayerPos());   //내와 타깃사이의 방향벡터

		vLook = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 1.f), vDir);

	}


	m_bSkill = false;
	m_bAttack = false;
	m_iNextAnimationIndex = ARI_ANIM_BTL_MOVE_RUN;
	m_bLoop = true;


	if (m_pVecMonsters->size() > 0)
	{
		if (fDistance > 1.2)
		{

			m_pTransformCom->Go_Straight(TimeDelta*0.4f);
			m_pTransformCom->Check_Right_Left(vDir, TimeDelta*0.6f);  //방향돌리는거 부드럽게. 
		}
		else {
			m_bPattern = false;
			return;
		}



	}



}

void CAlphen::Falling(_double TimeDelta)
{
	_vector  vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float vPosY = XMVectorGetY(vPos);
	if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_FALL)
	{
		//m_bMove = false;
		m_dFallSpeed += TimeDelta*m_dGravityPower;
		m_bLand = false;

		vPosY -= (_float)m_dFallSpeed;
		if (vPosY <= 0)
		{
			m_bLoop = false;
			vPosY = 0;
			m_dFallSpeed = 0.0;
			m_iNextAnimationIndex = ARI_ANIM_BTL_LAND;


		}
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, vPosY));
	}
	else if (m_iCurrentAnimationIndex == ARI_ANIM_BTL_LAND)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1)
		{
			m_bLand = true;
			m_bAir = false;
			m_bPattern = false;
			m_iAbleJumpCount = 1;
			m_dJumpSpeed = 0.03;
		}
	}
}

void CAlphen::AI(_double TimeDelta)
{
	if (!m_bPattern)
	{
		Pattern_Choice(); //패턴을 선택함
	}

	else // 위에서 패턴이 정해졌다면 이곳에서 수행한다.
	{


		switch (m_ePattern)
		{

		case Client::CAlphen::ARI_AI_CHASE:
			Chase(TimeDelta);
			break;

		case Client::CAlphen::ARI_AI_MOVE:
			Move(TimeDelta);
			break;

		case Client::CAlphen::ARI_AI_GROUNDBATTLE:
			GroundBattle(TimeDelta);
			break;

		case Client::CAlphen::ARI_AI_AIRBATTLE:
			AirBattle(TimeDelta);
			break;

		case Client::CAlphen::ARI_AI_FALL:
			Falling(TimeDelta);
			break;
		}

		if (m_bSkill)
		{
			//m_bSkillKeyDown = true;
			Skill(TimeDelta);
		}

	}
}

CAlphen * CAlphen::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CAlphen*   pInstance = new CAlphen(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CAlphen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAlphen::Clone(void * pArg)
{
	CAlphen*   pInstance = new CAlphen(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CAlphen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlphen::Free()
{
	__super::Free();
}