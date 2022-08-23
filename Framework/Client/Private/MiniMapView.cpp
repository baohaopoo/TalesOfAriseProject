#include "stdafx.h"
#include "MiniMapView.h"

//전투
MIniMapView::MIniMapView(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

MIniMapView::MIniMapView(const MIniMapView& rhs)
	:CUI(rhs)
{
}

HRESULT MIniMapView::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT MIniMapView::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}	

	
	m_iSprite = ((MINIMAPDESC*)pArg)->kind;

	m_iSprite = 0;
	m_iShaderPass =3;
	m_fDistanceToCamera = 110;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}



	return S_OK;
}

void MIniMapView::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);



	//////플레이어 위치
	CTransform* pPlayerTransform = (CTransform*)pGameInstnace->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_vector player = pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	//UI이니까 vUp에  Player를 둔다.

	_vector		vUp = XMVector3Normalize(XMVectorSet(XMVectorGetX(player), XMVectorGetZ(player), 0.f, 0.f));
	_vector		vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	_vector		vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));
	_float3 vScale = m_pTransformCom->Get_Scale();


	vRight = XMVector3Normalize(vRight) * vScale.x;
	vUp = XMVector3Normalize(vUp) * vScale.y;
	vLook = XMVector3Normalize(vLook) * vScale.z;


	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

	Safe_Release(pGameInstnace);
}

void MIniMapView::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT MIniMapView::Render()
{

	 
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	
	return S_OK;
}

HRESULT MIniMapView::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_MiniMapView", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To MIniMapView : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

MIniMapView * MIniMapView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	MIniMapView* pInstance = new MIniMapView(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To MIniMapView : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * MIniMapView::Clone(void * pArg)
{
	MIniMapView* pInstance = new MIniMapView(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To MIniMapView : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MIniMapView::Free()
{
}
