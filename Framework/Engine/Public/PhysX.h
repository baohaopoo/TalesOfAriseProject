#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CPhysX final : public CBase
{
	DECLARE_SINGLETON(CPhysX)

private:
	CPhysX();
	virtual ~CPhysX() = default;

public:
	HRESULT Initialize();
	HRESULT CreateScene();
	HRESULT CleanScene();
	
	enum GEOMETRY { GEOMETRY_SPHERE, GEOMETRY_BOX, GEOMETRY_CAPSULE, GEOMETRY_END };
public://물리 물체 생성
	PxRigidDynamic* CreateDynamicCollider(GEOMETRY Geometry, _float Size, _float3 Pos);
	PxRigidDynamic* CreateKnematicCollider(GEOMETRY Geometry, _float Size, _float3 Pos);
public://쿼리
	PxRaycastBuffer CreateRaycast(PxVec3 origin, PxVec3 unitDir, PxReal maxDistance,_bool Multi= false);//레이 충돌
	PxSweepBuffer CreateSweep(GEOMETRY Geometry, PxVec3 Geometrysize, PxTransform TF, PxVec3 unitDir, PxReal maxDistance, _bool Multi = false);//매쉬충돌
	PxOverlapBuffer CreateOverlap(GEOMETRY Geometry, PxVec3 Geometrysize, PxTransform TF, _bool Multi = false);
public://체인 생성
	void createChain(const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, CGameObject* User, _int Num = 0 ,PxRigidActor * a3 = NULL);
	PxJoint* createLimitedSpherical(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	PxJoint* createBreakableFixed(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	PxJoint* createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
public:

	void Tick(_double TimeDelta);
	HRESULT Render();

	PxControllerManager* Get_ControllerManager() { return gControllerManager; }
	PxMaterial* Get_Material() { return gMaterial; }
	PxScene* Get_Scene() { return gScene; }
	PxPhysics* Get_Physics() { return gPhysics; }




private:

	// Foundation을 생성하는데 필요한 변수
	PxDefaultAllocator gAllocator;
	PxDefaultErrorCallback gErrorCallback;
	PxFoundation* gFoundation = NULL;		// 음...필수??
	// PxFoundation이 있어야 gPhysics를 생성할 수 있다

	PxPhysics* gPhysics = NULL;				// 음...필수??
	// CPU리소스를 효율적으로 공유할 수 있도록 하기 위해 구현하는 것을 추천
	PxDefaultCpuDispatcher* gDispatcher = NULL;		// 음...필수??
	PxControllerManager* gControllerManager = NULL;
	PxScene* gScene = NULL;

	// gPhysics를 사용해 createMaterial해서 사용
	// 충돌제 마찰력, Dynamic 마찰력, 탄성력을 지정하여 사용
	PxMaterial* gMaterial = NULL;
	PxPvd* gPvd = NULL;

	PxReal stackZ = 10.0f;


public:
	virtual void Free() override;
};

END