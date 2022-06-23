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
	int					m_Type;

public:
	CEnemy();
	~CEnemy();
	void Initialize();
	void Start(const Vector3& p,int t = 0);
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
	void UpdateType0(CEnemyShot* shot, int smax);
	void UpdateBossParts(CEnemyShot* shot, int smax);
	void SetPosition(const Vector3& p) { m_Pos = p; }
	void SetRotation(const Vector3& r) { m_Rot = r; }
	CSphere GetSphere() {
		switch (m_Type)
		{
		case 1:
		case 2:
		case 3:
			return CSphere(m_Pos, 1.5f);
		}
		return CSphere(m_Pos, 0.5f);
	}
};