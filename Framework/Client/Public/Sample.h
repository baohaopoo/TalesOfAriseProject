#pragma once
#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)

class CSample final : public CPlayer,
	//////////PhysX �ʼ� ���//////////////////////////////
	public PxUserControllerHitReport,
	public PxControllerBehaviorCallback
	//////////PhysX �ʼ� ��� ��//////////////////////////////
{
	//////////PhysX �ʼ� ����//////////////////////////////
public:
	// ĸ�� ��Ʈ�ѷ� ����� �Լ� 
	HRESULT SetUp_Controller();
	//ĸ�� ��Ʈ�ѷ� �浹ó�� �Լ�
	virtual void onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void onControllerHit(const PxControllersHit& hit) override;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;
	//ĸ�� ��Ʈ�ѷ� �浹 ��� �÷���
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor)override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller)override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle)override;
private:
	PxCapsuleController* gController = nullptr;// ĸ�� ��Ʈ�Ѽ� ������
	PxRigidDynamic* ControllerActor = nullptr;// ĸ�� ���� ����
	//ControllerActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	//->��Ʈ�ѷ� ���� ����ȿ�� �޵��� ����� �Լ�(�̶� ��Ʈ�ѷ��� �����̴� �ƴ� ���͸� ����ȿ���� �ޱ⶧���� ������ ���������� ��Ʈ�ѷ��� �����������;

	//////////PhysX �ʼ� ���� ��//////////////////////////
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
	_uint m_NumAnim = 0;	// �ִϸ��̼� �� ���� �����ϱ� ���� ����
	PxRigidDynamic* Px = nullptr;//�Ϲ� ���� ��ü
	PxRigidDynamic* Px5 = nullptr;//�Ϲ� ���� ��ü
	PxRigidDynamic* A0 = nullptr;//������ ���� ��ü
	
};

END