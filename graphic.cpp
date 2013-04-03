// graphic.cpp : �O���t�B�b�N�֘A�̊֐���� (�C���[�W�{�^���p)

#include <windows.h>
#include <tchar.h>

#include <objidl.h>
#include <olectl.h>

#include "graphic.h"
#include "FileFunc.h"
#include "pecmd.h"

#define HIMETRIC_INCH   2540

// �摜�ǂݍ���
// LoadPictureOnResource �`���̃f�o�C�X�R���e�L�X�g�n���h��, ���\�[�X��, ���\�[�X�^�C�v
BOOL LoadPictureOnResource(HDC hdc_main, LPCTSTR res, LPCTSTR type)
{
	IStream *iStream=NULL;
	IPicture *iPicture;
	HRSRC hFind;
	HGLOBAL hLoad, hMem;
	DWORD nSize;
	HMODULE hInst = GetModuleHandle(NULL);
	HRESULT result;
	//bshort type;
	long Height, Width;
	int pic_x, pic_y;
	
	// ���\�[�X����f�[�^�̓ǂݏo��
	hFind = FindResource(hInst, res, type);
	if (hFind == NULL) return FALSE;
	hLoad = LoadResource(hInst, hFind);
	if (hLoad == NULL) return FALSE;
	nSize = SizeofResource(hInst, hFind);
	if (nSize == 0) return FALSE;
	hMem = GlobalAlloc(GHND, nSize);
	if (hMem == NULL) return FALSE;
	
	// �f�[�^�̃R�s�[
	CopyMemory(GlobalLock(hMem), LockResource(hLoad), nSize);
	GlobalUnlock(hMem);
	
	result = CreateStreamOnHGlobal(hMem, TRUE, &iStream);
	if (SUCCEEDED(result) == FALSE)
		return FALSE;
	
	// �摜�ǂݍ���
	result = OleLoadPicture(iStream, nSize, FALSE, IID_IPicture, (LPVOID*)&iPicture);
	
	iStream->Release();
	
	if (SUCCEEDED(result) == FALSE || iPicture == NULL)
		return FALSE;
	
	// �摜�̃T�C�Y�擾
	iPicture->get_Width(&Width);
	iPicture->get_Height(&Height);
	
	// �摜�̑傫���̒P�ʂ�ύX
	pic_x = MulDiv(Width, GetDeviceCaps(hdc_main, LOGPIXELSX), HIMETRIC_INCH);
	pic_y = MulDiv(Height, GetDeviceCaps(hdc_main, LOGPIXELSY), HIMETRIC_INCH);
	
	// �摜��]��
	iPicture->Render(hdc_main, 0, 0, pic_x, pic_y, 0, Height, Width, -Height, NULL);
	
	// ��n��
	iPicture->Release();
	GlobalFree(hMem);
	
	return TRUE;
}

// �t�H���_�A�C�R����`��
void DrawFolderImage(HDC hdc, int pos_y1, int pos_y2)
{
	SHFILEINFO shfinfo;
	
	ZeroMemory(&shfinfo, sizeof(SHFILEINFO)); // �ꉞ�[���N���A
	SHGetFileInfo(NULLTEXT, NULL, &shfinfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_SMALLICON);
	DrawIconEx(hdc, 2, pos_y1, shfinfo.hIcon, 16, 16, 0, NULL, DI_NORMAL); // �ʏ펞�̃A�C�R��
	DrawIconEx(hdc, 2, pos_y2, shfinfo.hIcon, 16, 16, 0, NULL, DI_NORMAL); // �}�E�X���{�^�����ɂ���Ƃ��̃A�C�R��
	DestroyIcon(shfinfo.hIcon);
}

// �t�@�C���A�C�R����`�� (��O����)
void DrawFileImage(HDC hdc, LPCTSTR szFile, int pos_x, int pos_y)
{
	TCHAR FileName[MAX_PATH + 1];
	
	// �t�@�C���p�X����t�@�C�������擾
	FilePathToFileName(szFile, FileName);

	// �]��
	if (!lstrcmp(FileName, TEXT("wpeutil_disablefirewall.cmd"))) {
		// ��O1
		BitBlt(hdc, pos_x-2, pos_y-2, 20, 20, hdc, 0, 0, SRCCOPY);
	} else if (!lstrcmp(FileName, TEXT("wpeutil_enablefirewall.cmd"))) {
		// ��O2
		BitBlt(hdc, pos_x-2, pos_y-2, 20, 20, hdc, 0, 20, SRCCOPY);
	} else {
		// �ʏ�
		SHFILEINFO shfinfo;
		ZeroMemory(&shfinfo, sizeof(SHFILEINFO));
		SHGetFileInfo(szFile, NULL, &shfinfo, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON); // �t�@�C���̃A�C�R�����擾
		DrawIconEx(hdc, pos_x, pos_y, shfinfo.hIcon, 16, 16, 0, NULL, DI_NORMAL); // �A�C�R����`��
		DestroyIcon(shfinfo.hIcon); // �A�C�R���n���h����j�� (��������Ȃ��ƈꕔ���ł͂��������Ȃ�炵���B)
	}
}

// �h��Ԃ� (hsp��boxf�Ƃقړ���)
void Boxfill(HDC hdc, int x1, int y1, int x2, int y2, HBRUSH hbr)
{
	// RECT�\���̂ɒl���Z�b�g
	RECT rect = {x1, y1, x2+1, y2+1};
	
	// �h��Ԃ�
	FillRect(hdc, &rect, hbr);
}

