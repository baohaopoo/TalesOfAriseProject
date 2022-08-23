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
public://���� ��ü ����
	PxRigidDynamic* CreateDynamicCollider(GEOMETRY Geometry, _float Size, _float3 Pos);
	PxRigidDynamic* CreateKnematicCollider(GEOMETRY Geometry, _float Size, _float3 Pos);
public://����
	PxRaycastBuffer CreateRaycast(PxVec3 origin, PxVec3 unitDir, PxReal maxDistance,_bool Multi= false);//���� �浹
	PxSweepBuffer CreateSweep(GEOMETRY Geometry, PxVec3 Geometrysize, PxTransform TF, PxVec3 unitDir, PxReal maxDistance, _bool Multi = false);//�Ž��浹
	PxOverlapBuffer CreateOverlap(GEOMETRY Geometry, PxVec3 Geometrysize, PxTransform TF, _bool Multi = false);
public://ü�� ����
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

	// Foundation�� �����ϴµ� �ʿ��� ����
	PxDefaultAllocator gAllocator;
	PxDefaultErrorCallback gErrorCallback;
	PxFoundation* gFoundation = NULL;		// ��...�ʼ�??
	// PxFoundation�� �־�� gPhysics�� ������ �� �ִ�

	PxPhysics* gPhysics = NULL;				// ��...�ʼ�??
	// CPU���ҽ��� ȿ�������� ������ �� �ֵ��� �ϱ� ���� �����ϴ� ���� ��õ
	PxDefaultCpuDispatcher* gDispatcher = NULL;		// ��...�ʼ�??
	PxControllerManager* gControllerManager = NULL;
	PxScene* gScene = NULL;

	// gPhysics�� ����� createMaterial�ؼ� ���
	// �浹�� ������, Dynamic ������, ź������ �����Ͽ� ���
	PxMaterial* gMaterial = NULL;
	PxPvd* gPvd = NULL;

	PxReal stackZ = 10.0f;


public:
	virtual void Free() override;
};

END