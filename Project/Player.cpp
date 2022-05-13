#include "Player.h"

/**
 * コンストラクタ
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_RotZ(0.0f){
}

/**
 * デストラクタ
 */
CPlayer::~CPlayer(){
}

/**
 * 読み込み
 */
bool CPlayer::Load(void){
	// メッシュの読み込み
	if (!m_Mesh.Load("player.mom"))
	{
		return false;
	}
	return true;
}

/**
 * 初期化
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -FIELD_HALF_Z + 2.0f);
	m_RotZ = 0;
}

/**
 * 更新
 */
void CPlayer::Update(void) {
	//回転方向
	float roll = 0;
	//キーボードでの移動
	if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		m_Pos.x = max(m_Pos.x - PLAYER_SPEED, -FIELD_HALF_X);
		roll -= MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		m_Pos.x = min(m_Pos.x + PLAYER_SPEED, FIELD_HALF_X);
		roll += MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_Pos.z = min(m_Pos.z + PLAYER_SPEED, FIELD_HALF_Z);
	}
	if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_Pos.z= max(m_Pos.z - PLAYER_SPEED, -FIELD_HALF_Z);
	}
	//回転
	float rotspeed = MOF_ToRadian(10);
	if (roll == 0)
	{
		rotspeed = min(abs(m_RotZ) * 0.1f, rotspeed);
	}
	if (abs(m_RotZ) <= rotspeed || signbit(m_RotZ) != signbit(roll))
	{
		m_RotZ += roll;
	}
	m_RotZ -= copysignf(min(rotspeed, abs(m_RotZ)), m_RotZ);
}

/**
 * 描画
 */
void CPlayer::Render(void){
	//ワールド行列作成
	CMatrix44 matworld;
	matworld.RotationZ(m_RotZ);
	matworld.SetTranslation(m_Pos);
	//メッシュの描画
	m_Mesh.Render(matworld);
}

/**
 * デバッグ文字描画
 */
void CPlayer::RenderDebugText(void){
	// 位置の描画
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"プレイヤー位置 X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
 * 解放
 */
void CPlayer::Release(void){
	m_Mesh.Release();
}