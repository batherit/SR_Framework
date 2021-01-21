#ifndef Export_Utility_h__
#define Export_Utility_h__

// Manage
#include "Management.h"
#include "KeyMgr.h"
#include "ProtoMgr.h"
#include "InputProcessor.h"
#include "InputProcessorMgr.h"
#include "LightMgr.h"
#include "CameraMgr.h"
#include "CollisionMgr.h"

// Object
#include "GameObject.h"
#include "ColliderObject_Sphere.h"
#include "ColliderObject_AABB.h"
#include "ColliderObject_OBB.h"
#include "Camera.h"
#include "CameraController.h"

// Components
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "PolygonRenderer.h"
#include "MeshRenderer.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "Physics.h"
#include "Optimization.h"
#include "Shader.h"

#include "RenderTargetMgr.h"

BEGIN(Engine)
// management
// Get
//inline CComponent*	   Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CLayer* GetLayer(const _tchar* _pLayerTag);

//inline HRESULT Create_Management(CManagement** ppManagement, LPDIRECT3DDEVICE9& pGraphicDev);
//inline HRESULT	SetUp_Scene(CScene* pScene);
//inline _int	Update_Scene(const _float& fTimeDelta);
//inline void	Render_Scene(void);

// protomgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*		Clone(const _tchar* pProtoTag);
inline CComponent*		GetOriProto(const _tchar* _pProtoTag);

// LightMgr
inline 	const D3DLIGHT9*		Get_Light(const _uint& iIndex);
inline HRESULT					Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
inline void						Render_Light(LPD3DXEFFECT& pEffect);
inline void						ClearLights();

// Renderer
inline CRenderer*		Get_Renderer(void);

// RenderTargetMgr
inline HRESULT		Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pTargetTag, const _uint& iWidth,
	const _uint& iHeight,
	D3DFORMAT Format,
	D3DXCOLOR Color);
inline HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

inline HRESULT		Begin_MRT(const _tchar* pMRTTag);
inline HRESULT		End_MRT(const _tchar* pMRTTag);

inline HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
inline void		Render_DebugBuffer(const _tchar* pMRTTag);
inline void		Throw_RenderTargetTexture(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantName);

inline void Release_Utility(void);

#include "Export_Utility.inl"
END

#endif // Export_Utility_h__
