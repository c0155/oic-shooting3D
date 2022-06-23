#pragma once

#include "GameDefine.h"
#include "Enemy.h"

// スクロール速度
#define		SCROLL_SPEED	(1.0f)

//敵出現情報構造体
typedef struct tag_ENEMYSTART{
	int			Count;
	float*		PosX;
	float*		Scroll;
}ENEMYSTART;

class CStage{
private:
	CSprite3D			m_BG;
	float				m_Scroll;
	ENEMYSTART*			m_pEnemyStart;
	CMeshContainer		m_EnemyMesh;
	int					m_EnemyNo;
public:
	CStage();
	~CStage();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	void Release();
	void Initialize(ENEMYSTART* pSt);
	void Update(CEnemy* ene, int ecnt);
	bool IsAllEnemy() { return m_EnemyNo >= m_pEnemyStart->Count; }
};