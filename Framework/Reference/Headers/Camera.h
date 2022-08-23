#pragma once

#include "GameObject.h"
//#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	enum TYPE { CAMERA, LIGHT, TYPE_END };
public:
	typedef struct tagCameraDesc
	{
		_float3		vEye, vAt, vAxisY;
		_float		fFovy, fNear, fFar, fAspect;
		TYPE eType;
	}CAMERADESC;

protected:
	explicit CCamera(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc = nullptr) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CAMERADESC						m_CameraDesc;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END