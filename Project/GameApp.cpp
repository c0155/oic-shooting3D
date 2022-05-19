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

CCamera				gcamera;
CDirectionalLight	glight;
CPlayer				gplayer;
CStage				gStage;
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

	//プレイヤーの初期化
	gplayer.Initialize();

	//ステージ初期化
	gStage.Initialize();
	
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
	gStage.Update();

	//プレイヤーの更新
	gplayer.Update();
	//デバッグ表示の切り替え
	if (g_pInput->IsKeyPush(MOFKEY_F1))
	{
		gddebug = ((gddebug) ? false : true);
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

	if (gddebug)
	{
		CMatrix44 matworld;
		matworld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matworld, Vector4(1, 1, 1, 0.4f));
	}

	g_pGraphics->SetDepthEnable(FALSE);

	if (gddebug)
	{
		//デバッグ文字
		gplayer.RenderDebugText();
		gStage.RenderDebugText();

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
	return TRUE;
}