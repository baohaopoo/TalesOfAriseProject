#pragma once
#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)

class CSample final : public CPlayer,
	//////////PhysX 필수 상속//////////////////////////////
	public PxUserControllerHitReport,
	public PxControllerBehaviorCallback
	//////////PhysX 필수 상속 끝//////////////////////////////
{
	//////////PhysX 필수 셋팅//////////////////////////////
public:
	// 캡슐 컨트롤러 만드는 함수 
	HRESULT SetUp_Controller();
	//캡슐 컨트롤러 충돌처리 함수
	virtual void onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void onControllerHit(const PxControllersHit& hit) override;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;
	//캡슐 컨트롤러 충돌 방식 플래그
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor)override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller)override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle)override;
private:
	PxCapsuleController* gController = nullptr;// 캡슐 컨트롤서 포인터
	PxRigidDynamic* ControllerActor = nullptr;// 캡슐 연동 엑터
	//ControllerActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	//->컨트롤러 액터 물리효과 받도록 만드는 함수(이때 컨트롤러의 움직이니 아닌 액터만 물리효과를 받기때문에 액터의 포지션으로 컨트롤러를 움직여줘야함;

	//////////PhysX 필수 셋팅 끝//////////////////////////
private:
	explicit CSample(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CSample(const CSample& rhs);
	virtual ~CSample() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void	Sample();

	
private:
	
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CSample* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float fJump = 0;
	_uint m_NumAnim = 0;	// 애니메이션 총 갯수 저장하기 위한 변수
	PxRigidDynamic* Px = nullptr;//일반 물리 객체
	PxRigidDynamic* Px5 = nullptr;//일반 물리 객체
	PxRigidDynamic* A0 = nullptr;//꼬리용 물리 객체
	
};

END