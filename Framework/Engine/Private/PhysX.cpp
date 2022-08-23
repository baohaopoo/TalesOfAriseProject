#include "PhysX.h"
#include "GameObject.h"
IMPLEMENT_SINGLETON(CPhysX)
//PX_C_EXPORT bool PX_CALL_CONV PxInitExtensions(physx::PxPhysics& physics, physx::PxPvd* pvd);
CPhysX::CPhysX()
{
}

HRESULT CPhysX::Initialize()
{
	
	// Foundation을 만들어 줍니다.
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation)
	{
		MSG_BOX(TEXT("PxCreateFoundation failed!"));
		return E_FAIL;
	}
	gPvd = PxCreatePvd(*gFoundation);
	if (!gPvd)
	{
		MSG_BOX(TEXT("gPvd failed!"));
		return E_FAIL;
	}

	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);

	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);//| PxPvdInstrumentationFlag::eDEBUG);

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	if (!gPhysics)
	{
		MSG_BOX(TEXT("gPhysics failed!"));
		return E_FAIL;
	}

	gPvd = PxCreatePvd(*gFoundation);

	PxInitExtensions(*gPhysics, gPvd);
	//PxRegisterArticulations(*gPhysics);
	//PxRegisterHeightFields(*gPhysics);

	return S_OK;
	//if (FAILED(CreateScene()))
	//{
	//	MSG_BOX(TEXT("CreateScene failed!"));
	//	return E_FAIL;
	//}
	//return S_OK;
}

HRESULT CPhysX::CreateScene()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	gDispatcher = PxDefaultCpuDispatcherCreate(info.dwNumberOfProcessors);

	//gDispatcher = PxDefaultCpuDispatcherCreate(2);	// Dispatcher를 Set 만들어 줍니다.

	// Scene을 Set 해줍니다.
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // 중력 세팅

	sceneDesc.cpuDispatcher = gDispatcher;

	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// 위에서 설정한대로 Scene을 만들어 줍니다.
	gScene = gPhysics->createScene(sceneDesc);
	/////
	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	/////

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);// 충돌체 마찰력, dynamic 마찰력, 탄성력


	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial); // 평지를 만들어 줍니다
	gScene->addActor(*groundPlane);

	gControllerManager = PxCreateControllerManager(*gScene);

	return S_OK;
}

HRESULT CPhysX::CleanScene()
{
	PX_RELEASE(gControllerManager);
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	return S_OK;
}

PxRigidDynamic * CPhysX::CreateDynamicCollider(GEOMETRY Geometry, _float Size, _float3 Pos)
{
	PxTransform transform(PxVec3(Pos.x, Pos.y, Pos.z));//, PxQuat());
	PxMaterial* pMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	PxShape* pShape;

	switch (Geometry)
	{
	case Engine::CPhysX::GEOMETRY_SPHERE:
		pShape = gPhysics->createShape(PxSphereGeometry(Size), *pMaterial, true);
		break;
	case Engine::CPhysX::GEOMETRY_BOX:
		pShape = gPhysics->createShape(PxBoxGeometry(Size, Size, Size), *pMaterial, true);
		break;
	case Engine::CPhysX::GEOMETRY_CAPSULE:
		pShape = gPhysics->createShape(PxCapsuleGeometry(Size, Size), *pMaterial, true);
		break;
	default:
		pShape = gPhysics->createShape(PxSphereGeometry(Size), *pMaterial, true);
		break;
	}
	pShape->setFlag(PxShapeFlag::eVISUALIZATION, true);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	//pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	PxRigidDynamic* pActor = gPhysics->createRigidDynamic(transform);
	pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	pActor->attachShape(*pShape);
	gScene->addActor(*pActor);

	return pActor;
}

PxRigidDynamic * CPhysX::CreateKnematicCollider(GEOMETRY Geometry, _float Size, _float3 Pos)
{
	PxTransform transform(PxVec3(Pos.x, Pos.y, Pos.z));//, PxQuat());
	PxMaterial* pMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);
	
	PxRigidDynamic* pActor;
	switch (Geometry)
	{
	case Engine::CPhysX::GEOMETRY_SPHERE:
		pActor = PxCreateKinematic(*gPhysics, transform, PxSphereGeometry(Size), *gMaterial, 1.0f);
		break;
	case Engine::CPhysX::GEOMETRY_BOX:
		pActor = PxCreateKinematic(*gPhysics, transform, PxBoxGeometry(Size, Size, Size), *gMaterial, 1.0f);
		break;
	case Engine::CPhysX::GEOMETRY_CAPSULE:
		pActor = PxCreateKinematic(*gPhysics, transform, PxCapsuleGeometry(Size, Size), *gMaterial, 1.0f);
		break;
	default:
		pActor = PxCreateKinematic(*gPhysics, transform, PxSphereGeometry(Size), *gMaterial, 1.0f);
		break;
	}

	//pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	//pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	gScene->addActor(*pActor);

	return pActor;
}

PxRaycastBuffer CPhysX::CreateRaycast(PxVec3 origin, PxVec3 unitDir, PxReal maxDistance, _bool Multi)
{
	//PxVec3 origin = PxVec3(1.f, 1.f, 1.f);                 // [in] 위치
	//PxVec3 unitDir = PxVec3(0.f, 1.f, 0.f);              // [in] 방향 노말
	//PxReal maxDistance = 3.f;            // [in] 최대 거리
	PxQueryFilterData  filterData(PxQueryFlag::eDYNAMIC);
	if (!Multi)
	{
		PxRaycastBuffer hit;                 // [out] 충돌 처리 정보
		bool status = gScene->raycast(origin, unitDir, maxDistance, hit, PxHitFlag::eDEFAULT, filterData);

		//if (status)
		//applyDamage(hit.block.position, hit.block.normal);
		if (status)
			return hit;
		else
			return NULL;
	}
	else
	{
		const PxU32 bufferSize = 256;        // [in] 버퍼 사이즈
		PxRaycastHit hitBuffer[bufferSize];  // [out] 충돌처리 배열
		PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] 충동처리 정보

		bool status = gScene->raycast(origin, unitDir, maxDistance, buf, PxHitFlag::eDEFAULT, filterData);
		//for (PxU32 i = 0; i < buf.nbTouches; i++)
		//	animateLeaves(buf.touches[i]);
		
		if (status)
			return buf;
		else
			return NULL;
	}

	
	
}

PxSweepBuffer  CPhysX::CreateSweep(GEOMETRY Geometry, PxVec3 Geometrysize, PxTransform TF,  PxVec3 unitDir, PxReal maxDistance, _bool Multi)
{
	PxQueryFilterData  filterData(PxQueryFlag::eDYNAMIC);
	PxGeometry* sweepShape = NULL;
	switch (Geometry)
	{
	case Engine::CPhysX::GEOMETRY_SPHERE:
	{
		sweepShape = &PxSphereGeometry(Geometrysize.x); // [in] 모양
		break;
	}
	case Engine::CPhysX::GEOMETRY_BOX:
	{
		sweepShape = &PxBoxGeometry(Geometrysize.x, Geometrysize.y, Geometrysize.z); // [in] 모양
		break;
	}
	case Engine::CPhysX::GEOMETRY_CAPSULE:
	{
		sweepShape = &PxCapsuleGeometry(Geometrysize.x, Geometrysize.y); // [in] 모양
		break;
	}
	case Engine::CPhysX::GEOMETRY_END:
	{
		sweepShape = &PxSphereGeometry(Geometrysize.x); // [in] 모양
		break;
	}
	default:
	{
		PxGeometry sweepShape = PxSphereGeometry(Geometrysize.x); // [in] 모양
		break;
	}
	}// [out] Sweep results
	    
	PxTransform initialPose = TF;  // [in] 위치 회전 상태
	PxVec3 sweepDirection = unitDir;    // [in] 뱡향

	if (!Multi)
	{
		PxSweepBuffer hit;
		bool status = gScene->sweep(*sweepShape, initialPose, sweepDirection, maxDistance, hit, PxHitFlag::eDEFAULT | PxHitFlag::eNORMAL, filterData);
		if (status)
			return hit;
		else
			return NULL;
	}
	else
	{
		const PxU32 bufferSize = 256;        // [in] 버퍼 사이즈
		PxSweepHit hitBuffer[bufferSize];  // [out] 충돌처리 배열
		PxSweepBuffer buf(hitBuffer, bufferSize); // [out] 충동처리 정보

		bool status = gScene->sweep(*sweepShape, initialPose, sweepDirection, maxDistance, buf, PxHitFlag::eDEFAULT | PxHitFlag::eNORMAL, filterData);
		if (status)
		{
			return buf;
		}	
		else
			return NULL;
	}
}


PxOverlapBuffer  CPhysX::CreateOverlap(GEOMETRY Geometry, PxVec3 Geometrysize, PxTransform TF, _bool Multi)
{
	PxQueryFilterData  filterData(PxQueryFlag::eDYNAMIC);
	//PxQueryFilterData  filterData(PxQueryFlag::eSTATIC);
	PxGeometry* overlapShape = NULL;
	switch (Geometry)
	{
	case Engine::CPhysX::GEOMETRY_SPHERE:
	{
		overlapShape = &PxSphereGeometry(Geometrysize.x); // [in] 모양
		break;
	}
	case Engine::CPhysX::GEOMETRY_BOX:
	{
		overlapShape = &PxBoxGeometry(Geometrysize.x, Geometrysize.y, Geometrysize.z); // [in] 모양
		break;
	}
	case Engine::CPhysX::GEOMETRY_CAPSULE:
	{
		overlapShape = &PxCapsuleGeometry(Geometrysize.x, Geometrysize.y); // [in] 모양
		break;
	}
	case Engine::CPhysX::GEOMETRY_END:
	{
		overlapShape = &PxSphereGeometry(Geometrysize.x); // [in] 모양
		break;
	}
	default:
	{
		PxGeometry sweepShape = PxSphereGeometry(Geometrysize.x); // [in] 모양
		break;
	}
	}// [out] Sweep results

	PxTransform initialPose = TF;  // [in] 위치 회전 상태
	PxOverlapBuffer hit;

	if (!Multi)
	{
		PxOverlapBuffer hit;
		bool status = gScene->overlap(*overlapShape, TF, hit, filterData);
		if (status)
			return hit;
		else
			return NULL;
	}
	else
	{
		const PxU32 bufferSize = 256;        // [in] 버퍼 사이즈
		PxOverlapHit hitBuffer[bufferSize];  // [out] 충돌처리 배열
		PxOverlapBuffer buf(hitBuffer, bufferSize); // [out] 충동처리 정보

		bool status = gScene->overlap(*overlapShape, TF, buf, filterData);
		if (status)
		{
			return buf;
		}
			
		else
			return NULL;

		
	}
}

void CPhysX::createChain(const PxTransform & t, PxU32 length, const PxGeometry & g, PxReal separation, CGameObject* User,_int Num, PxRigidActor * a3)
{
	PxVec3 offset(separation / 2, 0, 0);
	PxTransform localTm(offset);
	PxRigidActor* prev = a3;

	for (PxU32 i = 0; i<length; i++)
	{
		//PxRigidDynamic* current = PxCreateDynamic(*gPhysics, t*localTm, g, *gMaterial, 1.0f);
		PxRigidDynamic* current = PxCreateDynamic(*gPhysics, t*localTm, g, *gMaterial, 1.0f);
		current->userData = User;
		if(Num == 0)
			createLimitedSpherical(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		else if(Num == 1)
			createBreakableFixed(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		else if(Num == 2)
			createDampedD6(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		gScene->addActor(*current);
		prev = current;
		localTm.p.x += separation;
	}
}

PxJoint * CPhysX::createLimitedSpherical(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	PxSphericalJoint* j = PxSphericalJointCreate(*gPhysics, a0, t0, a1, t1);
	j->setLimitCone(PxJointLimitCone(PxPi / 4, PxPi / 4, 0.05f));
	j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
	return j;
}

PxJoint * CPhysX::createBreakableFixed(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	PxFixedJoint* j = PxFixedJointCreate(*gPhysics, a0, t0, a1, t1);
	//j->setBreakForce(1000, 100000);
	j->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
	j->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);
	return j;
}

PxJoint * CPhysX::createDampedD6(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	PxD6Joint* j = PxD6JointCreate(*gPhysics, a0, t0, a1, t1);
	j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
	j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 1000, FLT_MAX, true));
	return j;
}

void CPhysX::Tick(_double TimeDelta)
{
	if (gScene)
	{
		gScene->simulate(TimeDelta);
		gScene->fetchResults(true);
	}
}

HRESULT CPhysX::Render()
{
	return S_OK;
}



void CPhysX::Free()
{
	if (gControllerManager)
		PX_RELEASE(gControllerManager);
	if (gScene)
		PX_RELEASE(gScene);
	if (gDispatcher)
		PX_RELEASE(gDispatcher);

	PxCloseExtensions();
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
}
