#pragma once

#include "GameDefine.h"
#include "EnemyShot.h"

class CEnemy{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Rot;
	bool				m_bShow;

	float				m_AnimTime;
	int					m_HP;
	int					m_ShotWait;
	int					m_ShotWaitSet;
	Vector3				m_TargePos;

public:
	CEnemy();
	~CEnemy();
	void Initialize();
	void Start(const Vector3& p);
	void Update();
	void Render();
	void RenderDebug();
	CSphere GetSphere() { return CSphere(m_Pos, 0.5f); }
	void RenderDebugText(int i);
	void SetMesh(CMeshContainer* pm){ m_pMesh = pm; }
	void Damage(int dmg);
	bool GetShow(){ return m_bShow; }
	void SetTargetPos(const Vector3& t) { m_TargePos = t; }
	void Update(CEnemyShot* shot, int smax);
};