// controls.h

#pragma once

typedef struct IMGPOSINFO
{
	// IMGPOSINFO structure
	// �C���[�W�{�^���p�̉摜�̈ʒu�����i�[
	//

	short x, y;

} IMGPOSINFO;

typedef struct CONTROLINFO
{
	// CONTROLINFO structure
	// �R���g���[���̏����i�[
	//
	
	HWND hWnd_control;
	short hdc_id;
	IMGPOSINFO imgpos[3];
	short drawmode;
	short option;

} CONTROLINFO;

class Controls {
public:
	//////////////
	// Functions
	//
	
	Controls(void);
	~Controls(void);
	
	int CreateControls(HWND hWnd, HINSTANCE hInst);
	int GetResText(TCHAR *lpResBuf, int flag);
	void DrawImageButtonOnMouseEvent(HWND hWnd_button, int flag);
	void DrawImageButtonOnOwnerDraw(HWND hWnd_button, HDC hdc_button, UINT state);
	void SetTabKeyFocus(int key_flag);
	
	//////////////
	// Data
	//
	
	// SetRes�̃t���O
	BOOL bSetRes;
	
private:
	//////////////
	// Functions
	//
	
	void SetImageButtonInfo(int id, HWND hWnd_button, int hdc_imgid, int pos_x1, int pos_y1, int pos_x2, int pos_y2, int pos_x3, int pos_y3);
	void LoadButtonImages(HWND hWnd, int id);
	void DrawImageButton(CONTROLINFO *lpControlInfo, HDC hdc_button);
	
	//////////////
	// Data
	//

	CONTROLINFO *controlinfo;
	
	// GUI�t�H���g
	HFONT hGUIFont;
	
	// �쐬���ꂽ�{�^���̐�
	int controls_all;

	// ��͓x�̓��̓{�b�N�X
	HWND hRes1, hRes2;
	
	// �z��ł܂Ƃ߂�
	HDC hdc_mem[5];
};

extern Controls *ctls;
