#include "pch.h"
#include "Loading.h"

#include "Export_Function.h"
#include "Attribute.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();		
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);		// 크리티컬 섹션 생성

	// 쓰레드 생성 함수
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szLoading, L"버퍼 로딩중...");
	
	int i = 0;
	
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_SphereCol", Engine::BUFFER_SPHERECOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_CubeCol", Engine::BUFFER_CUBECOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"M_Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												Engine::RESOURCE_STATIC,
												L"Buffer_TerrainTex",
												Engine::BUFFER_TERRAINTEX,
												VTXCNTX,
												VTXCNTZ,
												VTXITV),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												Engine::RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);



	// 임시 리소스 로드.

	
	




	// 텍스쳐
	lstrcpy(m_szLoading, L"텍스처 로딩중...");
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Logo", Engine::TEX_NORMAL, L"../../Resource/Texture/Logo/Logo.jpg"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Player", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Ma.jpg"), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
												Engine::RESOURCE_STAGE,
												L"Texture_Terrain",
												Engine::TEX_NORMAL,
												L"../../Resource/Texture/Terrain/Grass_%d.tga", 2),
												E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_Logo", Engine::TEX_NORMAL, L"../../Resource/Texture/Logo/Logo.jpg"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Height", Engine::TEX_NORMAL, L"../../Resource/Texture/Terrain/Height2.bmp"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Texture_SkyBox",
		Engine::TEX_CUBE,
		L"../../Resource/Texture/SkyBox/burger%d.dds", 4),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"CobbledStreet_DM", Engine::TEX_NORMAL, L"../../Resource/Terrain/Texture/CobbledStreet_DM.tga"), E_FAIL);
	
	// UI
	// Cursor
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_Cursor", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_Cursor.tga"), E_FAIL);

	// HP
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_HP_Branch", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_HP_Branch.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		L"UI_HP",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_HP%d.tga", 8),
		E_FAIL);

	// Targeting
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_Targeting", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_Targeting.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_TargetingSide", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_TargetingSide.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_TargetingMid", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_TargetingMid.tga"), E_FAIL);

	// Aim
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_Aim", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_Aim.tga"), E_FAIL);

	// GunGauge
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_GunCover", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_GunCover.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_GunGauge", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_GunGauge.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_GunOverload", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_GunOverload.tga"), E_FAIL);

	// FadeIn/Out
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_Layer", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_Layer.png"), E_FAIL);
	
	// BunnyBomb
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_BunnyBomb", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_BunnyBomb.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_Arrow", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_Arrow.tga"), E_FAIL);

	// Title
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_AliceTitleBackPlane", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_AliceTitleBackPlane.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_RedCurve", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_RedCurve.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_NewGameOn", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_NewGameOn.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_NewGameOff", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_NewGameOff.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_GameExitOn", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_GameExitOn.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_GameExitOff", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_GameExitOff.png"), E_FAIL);
	
	// WeaponLock
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_WeaponLock_Board", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_WeaponLock_Board.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_WeaponLock_Tooth", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_WeaponLock_Tooth.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_WeaponLock_Lock", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_WeaponLock_Lock.png"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_WeaponLock_VorpalBlade", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_WeaponLock_VorpalBlade.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_WeaponLock_HobbyHorse", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_WeaponLock_HobbyHorse.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_WeaponLock_Gun", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_WeaponLock_Gun.tga"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_WeaponLock_BunnyBomb", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_WeaponLock_BunnyBomb.tga"), E_FAIL);


	lstrcpy(m_szLoading, L"메쉬 로딩중...");
	// UI
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"AliceTitleBackPlane",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/UI/",
		L"AliceTitleBackPlane.X"),
		E_FAIL);

	// Stone 메쉬 원본 생성
	/*FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Mesh_Stone",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/TombStone/",
		L"TombStone.X"),
		E_FAIL);*/

	// 스톤에 맞는 메쉬 콜라이더 원본 생성.
	/*Engine::CStaticMesh* pStaticMesh = nullptr;
	pStaticMesh = static_cast<Engine::CStaticMesh*>(Engine::GetOriResource(Engine::RESOURCE_STAGE, L"Mesh_Stone"));
	Engine::CComponent* pComponent = Engine::CMeshCollider::Create(m_pGraphicDev, pStaticMesh->Get_VtxPos(), pStaticMesh->Get_NumVtx(), pStaticMesh->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(L"Collider_Stone", pComponent);*/

	// Player 메쉬 원본 생성
	/*FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/PlayerXfile/",
		L"Player.X"),
		E_FAIL);*/

	// 보팔검 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"VorpalBlade",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/VorpalBlade/",
		L"VorpalBlade.X"),
		E_FAIL);

	// 말막대기 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"HobbyHorse",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/HobbyHorse/",
		L"HobbyHorse.X"),
		E_FAIL);

	// 후추분쇄기 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Gun",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Gun/",
		L"Gun.X"),
		E_FAIL);
	// 총알 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Bullet",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Bullet/",
		L"Bullet.X"),
		E_FAIL);

	// 토끼폭탄 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"BunnyBomb",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/BunnyBomb/",
		L"BunnyBomb.X"),
		E_FAIL);

	// 앨리스 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"AliceW",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Alice/",
		L"AliceW.X"),
		E_FAIL);

	// 포크 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Fork",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Fork/",
		L"Fork.X"),
		E_FAIL);

	// 낫 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Sickle",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Sickle/",
		L"Sickle.X"),
		E_FAIL);

	// 점프 패드 생성(Dynamic)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"JumpPad",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/JumpPad/",
		L"JumpPad.X"),
		E_FAIL);


	// HP Flower 생성(Dynamic)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"HPFlower",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/HPFlower/",
		L"HPFlower.X"),
		E_FAIL);

	// // Snail 생성(Static)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Snail",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Snail/",
		L"Snail.X"),
		E_FAIL);

	// Tooth 생성(Static)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Tooth",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Tooth/",
		L"Tooth.X"),
		E_FAIL);

	// Valve 생성(Static)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Valve",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Valve/",
		L"Valve.X"),
		E_FAIL);

	// 상호작용 발판류
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MirrorPad",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Platform/",
		L"MirrorPad.X"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MirrorPadFrame",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Platform/",
		L"MirrorPadFrame.X"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Platform",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Platform/",
		L"Platform.X"),
		E_FAIL);

	// 빅맨A
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"BigGuyA",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/BigGuy/",
		L"BigGuyA.X"),
		E_FAIL);

	// 빅맨B
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"BigGuyB",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/BigGuy/",
		L"BigGuyB.X"),
		E_FAIL);

	// Cat
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Cat",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Cat/",
		L"Cat.X"),
		E_FAIL);

	// 매드캡A
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MadCapA",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/MadCap/",
		L"MadCapA.X"),
		E_FAIL);

	// 나사 파리
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"BolterFly",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/BolterFly/",
		L"BolterFly.X"),
		E_FAIL);

	// 미니 그룬트
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MiniGrunt",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/MiniGrunt/",
		L"MiniGrunt.X"),
		E_FAIL);

	// 보스
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Boss",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Boss/",
		L"Boss.X"),
		E_FAIL);




	// Alice 메쉬 원본 생성
	/*FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Mesh_Alice",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Alice/",
		L"testalice.X"),
		E_FAIL);*/

	// 플레이어에 맞는 메쉬 콜라이더 원본 생성. (미구현)
	//Engine::CDynamicMesh* pDynamicMesh = nullptr;
	//pDynamicMesh = static_cast<Engine::CDynamicMesh*>(Engine::GetOriResource(Engine::RESOURCE_STAGE, L"Mesh_Player"));
	//pComponent = Engine::CMeshCollider::Create(m_pGraphicDev, pDynamicMesh->Get_VtxPos(), pDynamicMesh->Get_NumVtx(), pDynamicMesh->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//Engine::Ready_Proto(L"Collider_Stone", pComponent);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	Engine::RESOURCE_STAGE,
	//	L"Mesh_Sword",
	//	Engine::TYPE_STATIC,
	//	L"../../Resource/Mesh/StaticMesh/Sword/",
	//	L"Sword.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Tree",
	//	Engine::TYPE_STATIC,
	//	L"../../Resource/Mesh/StaticMesh/Tree/",
	//	L"Tree01.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Navi",
	//	Engine::TYPE_NAVI,
	//	NULL,
	//	NULL),
	//	E_FAIL);

	//Minge
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorbuildingE01_BlockA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"PoorbuildingE01_BlockA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorbuildingF01_BlockA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"PoorbuildingF01_BlockA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorbuildingF01_BlockB",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"PoorbuildingF01_BlockB.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorbuildingA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"PoorbuildingA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorbuildingB",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"PoorbuildingB.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorbuildingC",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"PoorbuildingC.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"BrickWallArch",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"BrickWallArch.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_90",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Curb_90.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_Section",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Curb_Section.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_Uneven",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Curb_Uneven.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Facade_Block_Greybrick",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Facade_Block_Greybrick.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Facade_BlockLow_Greybrick",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Facade_BlockLow_Greybrick.X"),
		E_FAIL);

	/*FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Facade_Block_Redbrick",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Facade_Block_Redbrick.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Facade_Block_Redbrick");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Facade_Block_Redbrick");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Facade_BlockLow_Redbrick",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Facade_BlockLow_Redbrick.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Facade_BlockLow_Redbrick");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Facade_BlockLow_Redbrick");*/

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"IronGateA_Fence",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"IronGateA_Fence.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"IronGateA_Gate",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"IronGateA_Gate.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Roadworks_Barrier",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Roadworks_Barrier.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Facade2_WallLong_StyleA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Minge/",
		L"Facade2_WallLong_StyleA.X"),
		E_FAIL);

	// Boss Stage
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"HatterFloorSystem_TeaMakerPlat",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/BossScene/",
		L"HatterFloorSystem_TeaMakerPlat.X"),
		E_FAIL);

	// 앨리스L
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"AliceL",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/AliceL/",
		L"AliceL.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Domino",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Domino/",
		L"Domino.X"),
		E_FAIL);

	// 돌과 나무들
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DecoMushroom_AlienGroup",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"DecoMushroom_AlienGroup.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarARockPillarA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarARockPillarA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"GiantMossRockA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"GiantMossRockA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MossRockA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"MossRockA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MossRockB",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"MossRockB.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"GiantOrientalRock_Wall",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"GiantOrientalRock_Wall.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarB",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarB.X"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarB2",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarB2.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarC",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarC.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"TearsRock",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"TearsRock.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"GloryTree_Base",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"GloryTree_Base.X"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MossRock_Walkable",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"MossRock_Walkable.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Keyhole_Glory",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"Keyhole_Glory.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DollSlide_CurveA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Slide/",
		L"DollSlide_CurveA.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DollSlide_CurveD",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Slide/",
		L"DollSlide_CurveD.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DollSlide_StraightA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Slide/",
		L"DollSlide_StraightA.X"),
		E_FAIL);
	
	lstrcpy(m_szLoading, L"엔터키를 누르십시오.");

	m_bFinish = true;


	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Safe_Release(pInstance);

	return pInstance;

}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

