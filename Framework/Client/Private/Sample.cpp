#include "stdafx.h"
#include "..\Public\Sample.h"
#include "GameInstance.h"


CSample::CSample(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CPlayer(pDeviceOut, pDeviceContextOut),
	PxUserControllerHitReport(),
	PxControllerBehaviorCallback()
{

}

CSample::CSample(const CSample & rhs)
	: CPlayer(rhs)
{
}

HRESULT CSample::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}
// ĸ�� ��Ʈ�ѷ� ����� �Լ� 
HRESULT CSample::SetUp_Controller()
{
	CPhysX*		pPhysX = GET_INSTANCE(CPhysX);
	PxCapsuleControllerDesc desc;
	_vector Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	desc.position = PxExtendedVec3(XMVectorGetX(Pos), XMVectorGetY(Pos), -XMVectorGetZ(Pos));
	desc.radius = 1.f;
	desc.height = 2.f;
	desc.stepOffset = 2.f;
	desc.volumeGrowth = 1.f;
	desc.slopeLimit = cosf(XMConvertToRadians(15.f));
	desc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	desc.upDirection = PxVec3(0.f, 1.f, 0.f);
	desc.contactOffset = 0.01f;
	PxMaterial* M = pPhysX->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	desc.material = M;
	desc.reportCallback = this;
	desc.behaviorCallback = this;
	desc.userData = this;
	gController = (PxCapsuleController*)pPhysX->Get_ControllerManager()->createController(desc);
	if (gController == nullptr)
		return E_FAIL;
	ControllerActor = gController->getActor();



	RELEASE_INSTANCE(CPhysX);
	return S_OK;
}
//ĸ�� ��Ʈ�ѷ� �浹ó�� �Լ�
void CSample::onShapeHit(const PxControllerShapeHit & hit)
{

}
void CSample::onControllerHit(const PxControllersHit & hit)
{
}
void CSample::onObstacleHit(const PxControllerObstacleHit & hit)
{
}
//ĸ�� ��Ʈ�ѷ� �浹 ��� �÷���
PxControllerBehaviorFlags CSample::getBehaviorFlags(const PxShape & shape, const PxActor & actor)
{
	//return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	//return PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
	//return PxControllerBehaviorFlag::eCCT_SLIDE;

	return PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
}
PxControllerBehaviorFlags CSample::getBehaviorFlags(const PxController & controller)
{
	return PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
}
PxControllerBehaviorFlags CSample::getBehaviorFlags(const PxObstacle & obstacle)
{
	return PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
}

HRESULT CSample::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimationIndex(m_iCurrentAnimationIndex);
////////////////��Ʈ�ѷ� �����Լ� �ʼ� ///////////////////
	SetUp_Controller();
//////////////////////////////////////////////////////////
	CPhysX*		pPhysX = GET_INSTANCE(CPhysX);
//���� ��ü ����
#pragma region 

	PxRigidDynamic * Px1 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_BOX, 2.f, _float3(15.f, 7.f, 0.f));
	Px1->userData = this;
	PxRigidDynamic * Px2 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_CAPSULE, 1.f, _float3(0.f, 2.f, 0.f));
	Px2->userData = this;
	PxRigidDynamic * Px3 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_BOX, 0.3f, _float3(10.f, 1.f, 0.f));
	Px3->userData = this;
	PxRigidDynamic * Px4 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_BOX, 0.5f, _float3(5.f, 1.f, -3.f));
	Px4->userData = this;
	PxRigidDynamic * Px5 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_BOX, 1.f, _float3(7.f, 7.f, 1.f));
	Px5->userData = this;
#pragma endregion
//���� ��ü ���� END

//���� ��ü ���� �Լ�
#pragma region 
	Px1->putToSleep();// ��ü ����(����� �������� ���� ����, ����� �������� �̵� ����)
	Px1->addForce(PxVec3(0.f,0.f,0.f)); // PxVec3 ����� ������ ��
	Px1->getGlobalPose(); // ���� ���¸� ������ ��(PxTransform.p�� PxVec3 ��ġ, PxTransform.q�� PxVec4 ���ʹϾ�)
	Px1->setGlobalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f))); // ���� ���� ��(PxTransform.p�� PxVec3 ��ġ, PxTransform.q�� PxVec4 ���ʹϾ�)
#pragma endregion
//���� ��ü ���� �Լ� END
	A0 = pPhysX->CreateKnematicCollider(CPhysX::GEOMETRY_SPHERE, 0.3f,_float3(1.f,1.f,1.f));
	//ü�� ���� �Լ�
	pPhysX->createChain(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)), 5, PxBoxGeometry(0.4f, 0.1f, 0.1f), 1.0f,this, 1, A0);
	RELEASE_INSTANCE(CPhysX);


	return S_OK;
}

void CSample::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CPhysX*		pPhysX = GET_INSTANCE(CPhysX);

	if (pGameInstance->Key_Down(DIK_1))
	{
		if (m_NumAnim - 1 > m_iNextAnimationIndex)
			++m_iNextAnimationIndex;
	}
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	

	

	//��Ʈ�ѷ� ������ ��Ʈ��
#pragma region 

	if (GetKeyState(VK_UP) & 0x8000)
	{
		gController->move(PxVec3(0.f, 0.001f, -10.f)*(_float)TimeDelta, 0, PxF32(TimeDelta), PxControllerFilters());
		//move(1.�̵����� �� �Ÿ�(PxVec3 �ڷ���), 2.�ּ� �̵��Ÿ�("1."�� �Ÿ��� �� ������ ������ �̵�X) 3.�� ��� �� ��� �ð�(PxF32 �ڷ���), ����());
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		gController->move(PxVec3(0.f, 0.001f, 10.f)*(_float)TimeDelta, 0, PxF32(TimeDelta), PxControllerFilters());
		ControllerActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		gController->move(PxVec3(10.f, 0.001f, 0.f)*(_float)TimeDelta, 0, PxF32(TimeDelta), PxControllerFilters());
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		gController->move(PxVec3(-10.f, 0.001f, 0.f)*(_float)TimeDelta, 0, PxF32(TimeDelta), PxControllerFilters());
	}

	gController->move(PxVec3(0.f, 1.f, 0.f)* (_float)TimeDelta * fJump, 0, PxF32(TimeDelta), PxControllerFilters());
	fJump -= (_float)TimeDelta * 100.f;
	PxControllerState m_PxState;
	gController->getState(m_PxState);// ��Ʈ�ѷ� �浹 ���� �޾ƿ��� �Լ�

		//��Ʈ�ѷ� ��ġ �޾ƿͼ� ĳ���Ϳ� ����
	PxExtendedVec3 Pos = gController->getFootPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(Pos.x, Pos.y, -Pos.z, 1.f));
		//��Ʈ�ѷ� ��ġ �޾ƿͼ� ĳ���Ϳ� ���� END

	
	if (m_PxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_DOWN || m_PxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_UP)
	{
		fJump = 0.f;
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			fJump = 30.f;
			
		}
	}

#pragma endregion
	//��Ʈ�ѷ� ������ ��Ʈ�� END
	
	

	//���� ��ġ
#pragma region 
	_vector Pos1 = XMVectorSet(0.f, 1.f, 2.f, 1.f);
	Pos1 = XMVector3TransformCoord(Pos1, m_pTransformCom->Get_WorldMatrix());
	A0->setGlobalPose(PxTransform(XMVectorGetX(Pos1), XMVectorGetY(Pos1), XMVectorGetZ(-Pos1)));
#pragma endregion
	//���� ��ġ END

	//���� �浹ü ����
#pragma region 

	if (GetKeyState('Q') & 0x8000)// �������� ����
	{
		PxRigidDynamic * Px0 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_SPHERE, 0.3f, _float3(Pos.x, Pos.y + 0.6f, Pos.z - 1.f));
		Px0->addForce(PxVec3(0.f, 0.f, 10.f));
	}

	if (GetKeyState('E') & 0x8000)// �Ʒ����� ����
	{
		
		PxRigidDynamic * Px0 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_SPHERE, 0.3f, _float3(Pos.x, Pos.y, Pos.z));
		Px0->addForce(PxVec3(0.f, 1.f, 0.f));
	}
#pragma endregion
	//���� �浹ü ���� END


	//���� ���� Sweep
#pragma region 
	if (GetKeyState('Z') & 0x8000)
	{
		_bool Multi = true;//����� (����, �ټ�) 
		PxSweepBuffer BS = pPhysX->CreateSweep(CPhysX::GEOMETRY_SPHERE, PxVec3(20.f, 20.f, 20.f), PxTransform(PxVec3(Pos.x, Pos.y, Pos.z)), PxVec3(0.f, 0.f, 1.f), 0.01f, Multi);
		
		if (Multi == false)//��Ƽ false
		{
			if (BS.block.actor != nullptr)
			{
				PxRigidActor* BT = BS.block.actor;
				PxRigidDynamic* BR = static_cast<PxRigidDynamic*>(BT);
				if (BR)
					BR->addForce(PxVec3(0.f, 20.f, 0.f));
			}
		}
		
		
		else if (Multi == true)//��Ƽ true
		{
			for (PxU32 i = 0; i < BS.nbTouches; i++)
				if (BS.touches != nullptr)
				{
					
					PxRigidActor* BT = BS.touches[i].actor;
					PxRigidDynamic* BR = static_cast<PxRigidDynamic*>(BT);
					if (BR)
						BR->addForce(PxVec3(0.f, 10.f, 0.f));
				}
				else
					break;
		}
		
	}
#pragma endregion
	//���� ���� END

	//���� �����ɽ�Ʈ raycast
#pragma region
	if (GetKeyState('X') & 0x8000)
	{
		_bool Multi = true;//����� (����, �ټ�) 
		PxRaycastBuffer B1 = pPhysX->CreateRaycast(PxVec3(Pos.x, Pos.y + 2.f, Pos.z-2.f),PxVec3(0.f, 0.f, -1.f),1000.f,Multi);
		//if(B1 == 0)
		//((PxRigidBody*)(B1.block.shape)->getActor())->addForce(PxVec3(0.f, 0.f, -1000.f));
		if (Multi == false)//��Ƽ false
		{
			if (B1.block.actor != nullptr)
			{
				PxRigidActor* BT = B1.block.actor;
				PxRigidDynamic* BR = static_cast<PxRigidDynamic*>(BT);
				if (BR)
					BR->addForce(PxVec3(0.f, 0.f, -20.f));

			//	if(BR->userData!=nullptr)
			//		((CSample*)BR->userData)->Sample();  ���Ϳ� ������ ������Ʈ ��ü �������� ���
			}
		}


		else if (Multi == true)//��Ƽ true
		{
			for (PxU32 i = 0; i < B1.nbTouches; i++)
				if (B1.touches != nullptr)
				{
					
					PxRigidActor* BT = B1.touches[i].actor;
					PxRigidDynamic* BR = static_cast<PxRigidDynamic*>(BT);
					//if (BR->userData != nullptr)
					//	((CSample*)BR->userData)->Sample(); ���Ϳ� ������ ������Ʈ ��ü �������� ���
					if (BR)
						BR->addForce(PxVec3(0.f, 0.f, -20.f));
				}
				else
					break;
			
		}
		
	}
#pragma endregion
	//���� �����ɽ�ƮEND
	
	RELEASE_INSTANCE(CPhysX);
	RELEASE_INSTANCE(CGameInstance);
}

void CSample::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

#ifdef _DEBUG
	m_pRendererCom->Add_DebugComponent(m_pOBBCom);
	m_pRendererCom->Add_DebugComponent(m_pSphereCom);
#endif // _DEBUG
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CSample::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		//if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
		//	return E_FAIL;
		//���� ��� �ؽ��İ� ��� ������ ����
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;

	}

#ifdef _DEBUG
	m_pOBBCom->Render();
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CSample::Sample()
{
	fJump = 50.f;
}





HRESULT CSample::SetUp_Components()
{
	///* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Fiona"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	ColliderDesc.vSize = _float3(0.7f, 1.4f, 0.7f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CSample::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

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

CSample * CSample::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CSample*	pInstance = new CSample(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CSample"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSample::Clone(void * pArg)
{
	CSample*	pInstance = new CSample(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CSample"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSample::Free()
{
	//////////��Ʈ�ѷ� ����//////////////
	ControllerActor->release();
	gController->release();


	__super::Free();
}