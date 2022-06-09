/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include	"Stage.h"
#include	"Stage1.h"

CCamera				gcamera;
CDirectionalLight	glight;
CPlayer				gplayer;
#define				ENEMY_COUNT			(20)
CEnemy				gEnemyArray[ENEMY_COUNT];
CStage				gStage;
//�f�o�b�O�\���t���O
bool				gddebug;

CVector3			gCPos;
CVector3			gTPos;
CVector3			gUVec;

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");

	//�J����������
	gcamera.SetViewPort();
	gcamera.LookAt(Vector3(0, 6.0f, -17.0f), Vector3(0, 0, -10), Vector3(0, 1, 0));
	gcamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gcamera);

	//���C�g������
	glight.SetDirection(Vector3(-1, -2, 1.5f));
	glight.SetDiffuse(MOF_COLOR_BLUE);
	glight.SetAmbient(MOF_COLOR_HWHITE);
	glight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&glight);



	//�v���C���[�̑f�ޓǂݍ���
	gplayer.Load();

	//�X�e�[�W
	gStage.Load();

	//�v���C���[�̏�����
	gplayer.Initialize();

	//�X�e�[�W������
	gStage.Initialize(&gStg1EnemyStart);

	//�G������
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Initialize();
	}
	
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();
	//�X�e�[�W�X�V
	gStage.Update(gEnemyArray,ENEMY_COUNT);

	//�v���C���[�̍X�V
	gplayer.Update();
	//�G�X�V
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Update();
	}
	//�G�Ƃ̓����蔻��
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gplayer.CollisionEnamy(gEnemyArray[i]);
	}
	//�f�o�b�O�\���̐؂�ւ�
	if (g_pInput->IsKeyPush(MOFKEY_F1))
	{
		gddebug = ((gddebug) ? false : true);
	}
	//�Q�[���I�[�o�[�\���㏉����
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && gplayer.IsDead())
	{
		//�I�u�W�F�N�g������
		gplayer.Initialize();
		gStage.Initialize(&gStg1EnemyStart);
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].Initialize();
		}
	}
	//�v���C���[�̓����ɍ��킹�ăJ�����𓮂���
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
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	// ��ʂ̃N���A
	g_pGraphics->ClearTarget(0.65f,0.65f,0.67f,0.0f,1.0f,0);

	g_pGraphics->SetDepthEnable(TRUE);

	gStage.Render();

	gplayer.Render();

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Render();
	}

	if (gddebug)
	{
		gplayer.RenderDebug();

		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebug();
		}

		CMatrix44 matworld;
		matworld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matworld, Vector4(1, 1, 1, 0.4f));
	}

	g_pGraphics->SetDepthEnable(FALSE);

	if (gddebug)
	{
		//�f�o�b�O����
		gplayer.RenderDebugText();
		gStage.RenderDebugText();
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebugText(i);
		}
	}

	if (gplayer.IsDead())
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED, "�Q�[���I�[�o�[ : Enter�L�[�ł�����x�ŏ�����");
	}
	
	// �`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gplayer.Release();
	gStage.Release();
	return TRUE;
}