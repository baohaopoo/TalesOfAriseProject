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
// 캡슐 컨트롤러 만드는 함수 
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
//캡슐 컨트롤러 충돌처리 함수
void CSample::onShapeHit(const PxControllerShapeHit & hit)
{

}
void CSample::onControllerHit(const PxControllersHit & hit)
{
}
void CSample::onObstacleHit(const PxControllerObstacleHit & hit)
{
}
//캡슐 컨트롤러 충돌 방식 플래그
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
////////////////컨트롤러 생성함수 필수 ///////////////////
	SetUp_Controller();
//////////////////////////////////////////////////////////
	CPhysX*		pPhysX = GET_INSTANCE(CPhysX);
//물리 물체 생성
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
//물리 물체 생성 END

//물리 물체 내부 함수
#pragma region 
	Px1->putToSleep();// 물체 슬림(충격이 가해지면 깨서 적용, 충격이 없을때는 이동 안함)
	Px1->addForce(PxVec3(0.f,0.f,0.f)); // PxVec3 방향과 힘으로 밈
	Px1->getGlobalPose(); // 월드 상태를 가지고 옴(PxTransform.p는 PxVec3 위치, PxTransform.q는 PxVec4 쿼터니언)
	Px1->setGlobalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f))); // 월드 상태 셋(PxTransform.p는 PxVec3 위치, PxTransform.q는 PxVec4 쿼터니언)
#pragma endregion
//물리 물체 내부 함수 END
	A0 = pPhysX->CreateKnematicCollider(CPhysX::GEOMETRY_SPHERE, 0.3f,_float3(1.f,1.f,1.f));
	//체인 생성 함수
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

	

	

	//컨트롤러 움직임 컨트롤
#pragma region 

	if (GetKeyState(VK_UP) & 0x8000)
	{
		gController->move(PxVec3(0.f, 0.001f, -10.f)*(_float)TimeDelta, 0, PxF32(TimeDelta), PxControllerFilters());
		//move(1.이동방향 및 거리(PxVec3 자료형), 2.최소 이동거리("1."의 거리가 이 값보다 낮으면 이동X) 3.전 명령 후 경과 시간(PxF32 자료형), 필터());
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
	gController->getState(m_PxState);// 컨트롤러 충돌 상태 받아오는 함수

		//컨트롤러 위치 받아와서 캐릭터에 적용
	PxExtendedVec3 Pos = gController->getFootPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(Pos.x, Pos.y, -Pos.z, 1.f));
		//컨트롤러 위치 받아와서 캐릭터에 적용 END

	
	if (m_PxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_DOWN || m_PxState.collisionFlags == PxControllerCollisionFlag::eCOLLISION_UP)
	{
		fJump = 0.f;
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			fJump = 30.f;
			
		}
	}

#pragma endregion
	//컨트롤러 움직임 컨트롤 END
	
	

	//꼬리 위치
#pragma region 
	_vector Pos1 = XMVectorSet(0.f, 1.f, 2.f, 1.f);
	Pos1 = XMVector3TransformCoord(Pos1, m_pTransformCom->Get_WorldMatrix());
	A0->setGlobalPose(PxTransform(XMVectorGetX(Pos1), XMVectorGetY(Pos1), XMVectorGetZ(-Pos1)));
#pragma endregion
	//꼬리 위치 END

	//원형 충돌체 생성
#pragma region 

	if (GetKeyState('Q') & 0x8000)// 전방으로 생성
	{
		PxRigidDynamic * Px0 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_SPHERE, 0.3f, _float3(Pos.x, Pos.y + 0.6f, Pos.z - 1.f));
		Px0->addForce(PxVec3(0.f, 0.f, 10.f));
	}

	if (GetKeyState('E') & 0x8000)// 아래으로 생성
	{
		
		PxRigidDynamic * Px0 = pPhysX->CreateDynamicCollider(CPhysX::GEOMETRY_SPHERE, 0.3f, _float3(Pos.x, Pos.y, Pos.z));
		Px0->addForce(PxVec3(0.f, 1.f, 0.f));
	}
#pragma endregion
	//원형 충돌체 생성 END


	//쿼리 스왑 Sweep
#pragma region 
	if (GetKeyState('Z') & 0x8000)
	{
		_bool Multi = true;//결과값 (단일, 다수) 
		PxSweepBuffer BS = pPhysX->CreateSweep(CPhysX::GEOMETRY_SPHERE, PxVec3(20.f, 20.f, 20.f), PxTransform(PxVec3(Pos.x, Pos.y, Pos.z)), PxVec3(0.f, 0.f, 1.f), 0.01f, Multi);
		
		if (Multi == false)//멀티 false
		{
			if (BS.block.actor != nullptr)
			{
				PxRigidActor* BT = BS.block.actor;
				PxRigidDynamic* BR = static_cast<PxRigidDynamic*>(BT);
				if (BR)
					BR->addForce(PxVec3(0.f, 20.f, 0.f));
			}
		}
		
		
		else if (Multi == true)//멀티 true
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
	//쿼리 스왑 END

	//쿼리 레이케스트 raycast
#pragma region
	if (GetKeyState('X') & 0x8000)
	{
		_bool Multi = true;//결과값 (단일, 다수) 
		PxRaycastBuffer B1 = pPhysX->CreateRaycast(PxVec3(Pos.x, Pos.y + 2.f, Pos.z-2.f),PxVec3(0.f, 0.f, -1.f),1000.f,Multi);
		//if(B1 == 0)
		//((PxRigidBody*)(B1.block.shape)->getActor())->addForce(PxVec3(0.f, 0.f, -1000.f));
		if (Multi == false)//멀티 false
		{
			if (B1.block.actor != nullptr)
			{
				PxRigidActor* BT = B1.block.actor;
				PxRigidDynamic* BR = static_cast<PxRigidDynamic*>(BT);
				if (BR)
					BR->addForce(PxVec3(0.f, 0.f, -20.f));

			//	if(BR->userData!=nullptr)
			//		((CSample*)BR->userData)->Sample();  엑터에 소유된 오브젝트 객체 가져오는 방법
			}
		}


		else if (Multi == true)//멀티 true
		{
			for (PxU32 i = 0; i < B1.nbTouches; i++)
				if (B1.touches != nullptr)
				{
					
					PxRigidActor* BT = B1.touches[i].actor;
					PxRigidDynamic* BR = static_cast<PxRigidDynamic*>(BT);
					//if (BR->userData != nullptr)
					//	((CSample*)BR->userData)->Sample(); 엑터에 소유된 오브젝트 객체 가져오는 방법
					if (BR)
						BR->addForce(PxVec3(0.f, 0.f, -20.f));
				}
				else
					break;
			
		}
		
	}
#pragma endregion
	//쿼리 레이케스트END
	
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
		//아직 노멀 텍스쳐가 없어서 띄울수가 없다
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
	//////////컨트롤러 삭제//////////////
	ControllerActor->release();
	gController->release();


	__super::Free();
}