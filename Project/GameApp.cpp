/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include	"Stage.h"
#include	"Stage1.h"
#include	"Boss.h"

CCamera				gcamera;
CDirectionalLight	glight;
CPlayer				gplayer;
#define				ENEMY_COUNT			(20)
CEnemy				gEnemyArray[ENEMY_COUNT];
#define				ENEMYSHOT_COUNT		(200)
CEnemyShot			gShotArray[ENEMYSHOT_COUNT];
CMeshContainer		gEnemyShotMesh;
CBoss				gBoss;
CStage				gStage;
bool				gbEnemyDestroyed;
bool				gbClear = false;
//デバッグ表示フラグ
bool				gddebug;

CVector3			gCPos;
CVector3			gTPos;
CVector3			gUVec;

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");

	//カメラ初期化
	gcamera.SetViewPort();
	gcamera.LookAt(Vector3(0, 6.0f, -17.0f), Vector3(0, 0, -10), Vector3(0, 1, 0));
	gcamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gcamera);

	//ライト初期化
	glight.SetDirection(Vector3(-1, -2, 1.5f));
	glight.SetDiffuse(MOF_COLOR_BLUE);
	glight.SetAmbient(MOF_COLOR_HWHITE);
	glight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&glight);



	//プレイヤーの素材読み込み
	gplayer.Load();

	//ステージ
	gStage.Load();

	//敵弾素材
	if (!gEnemyShotMesh.Load("eshot.mom"))
	{
		return false;
	}
	gBoss.Load();

	//プレイヤーの初期化
	gplayer.Initialize();

	//ステージ初期化
	gStage.Initialize(&gStg1EnemyStart);

	//敵初期化
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Initialize();
	}

	//敵弾初期化
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Initialize();
		gShotArray[i].SetMesh(&gEnemyShotMesh);
	}
	gBoss.Initialize();
	
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();
	//ステージ更新
	gStage.Update(gEnemyArray,ENEMY_COUNT);

	//プレイヤーの更新
	gplayer.Update();
	//敵更新
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].SetTargetPos(gplayer.GetPosition());
		gEnemyArray[i].Update(gShotArray, ENEMYSHOT_COUNT);
	}
	//ボス更新
	gBoss.SetTargetPos(gplayer.GetPosition());
	gBoss.Update(gShotArray, ENEMYSHOT_COUNT);
	//敵弾更新
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Update();
	}
	//敵との当たり判定
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gplayer.CollisionEnamy(gEnemyArray[i]);
	}
	//ボス当たり判定
	gplayer.CollisionBoss(gBoss);
	//敵弾当たり判定
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gplayer.CollisionEnemyShot(gShotArray[i]);
	}
	//雑魚全滅判定
	if (!gbEnemyDestroyed && gStage.IsAllEnemy())
	{
		bool btmp = true;
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (gEnemyArray[i].GetShow())
			{
				btmp = false;
			}
		}
		gbEnemyDestroyed = btmp;
		if (gbEnemyDestroyed)
		{
			gBoss.Start();
			gBoss.SetTargetPos(gplayer.GetPosition());
			gBoss.Update(gShotArray, ENEMY_COUNT);
		}
	}
	if (!gbClear && gbEnemyDestroyed && !gBoss.GetShow())
	{
		gbClear = true;
	}
	//デバッグ表示の切り替え
	if (g_pInput->IsKeyPush(MOFKEY_F1))
	{
		gddebug = ((gddebug) ? false : true);
	}
	//ゲームオーバー表示後初期化
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && ( gplayer.IsDead() || gbClear))
	{
		gbEnemyDestroyed = false;
		gbClear = false;
		//オブジェクト初期化
		gplayer.Initialize();
		gStage.Initialize(&gStg1EnemyStart);
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].Initialize();
		}
		for (int i = 0; i < ENEMYSHOT_COUNT; i++)
		{
			gShotArray[i].Initialize();
		}
		gBoss.Initialize();
	}
	//プレイヤーの動きに合わせてカメラを動かす
	float posx = gplayer.GetPosition().x * 0.4f;
	CVector3 cpos = gcamera.GetViewPosition();
	CVector3 tpos = gcamera.GetTargetPosition();
	CVector3 vup = Vector3(0, 1, 0);
	cpos.x = posx;
	tpos.x = posx;
	vup.RotationZ(gplayer.GetPosition().x / FIELD_HALF_X * MOF_ToRadian(10.0f));
	gcamera.LookAt(cpos, tpos, vup);
	gcamera.Update();
	return TRUE;
}

/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画処理
	g_pGraphics->RenderStart();
	// 画面のクリア
	g_pGraphics->ClearTarget(0.65f,0.65f,0.67f,0.0f,1.0f,0);

	g_pGraphics->SetDepthEnable(TRUE);

	gStage.Render();

	gplayer.Render();

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Render();
	}
	//ボス描画
	gBoss.Render();
	//敵弾描画
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Render();
	}

	if (gddebug)
	{
		gplayer.RenderDebug();

		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebug();
		}
		//ボスデバッグ
		gBoss.RenderDebug();
		
		//敵弾デバッグ
		for (int i = 0; i < ENEMYSHOT_COUNT; i++)
		{
			gShotArray[i].RenderDebug();
		}

		CMatrix44 matworld;
		matworld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matworld, Vector4(1, 1, 1, 0.4f));
	}

	g_pGraphics->SetDepthEnable(FALSE);

	if (gddebug)
	{
		//デバッグ文字
		gStage.RenderDebugText();
		gplayer.RenderDebugText();
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebugText(i);
		}
	}

	if (gplayer.IsDead())
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "ゲームオーバー : Enterキーでもう一度最初から");
	}
	else if(gbClear)
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "ゲームクリア　：　Enterキーでもう一度最初から");
	}
	
	// 描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gplayer.Release();
	gStage.Release();
	gEnemyShotMesh.Release();
	gBoss.Release();
	return TRUE;
}