#pragma once

#include "GameDefine.h"
#include "PlayerShot.h"
#include "Enemy.h"
#include "Boss.h"

// ˆÚ“®‘¬“x
#define PLAYER_SPEED		(0.1f)

//”­ŽËŒÀŠE
#define PLAYERSHOT_COUNT	(40)

//”­ŽËŠÔŠu
#define PLAYERSHOT_WAIT		(5)

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_Pos;
	float			m_RotZ;
	CMeshContainer  m_ShotMesh;
	CPlayerShot     m_ShotArray[PLAYERSHOT_COUNT];
	int				m_ShotWait;
	bool			m_bDead;
public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug();
	CSphere GetSphere() { return CSphere(m_Pos, 0.4f); }
	void RenderDebugText();
	const CVector3 GetPosition(){ return m_Pos; }
	void CollisionEnamy(CEnemy& ene);
	bool IsDead() { return m_bDead; }
	void CollisionEnemyShot(CEnemyShot& shot);
	void CollisionBoss(CBoss& boss);
	void Release();
};