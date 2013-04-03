// FileFunc.cpp : �t�@�C���֘A�̊֐����

#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#include "FileFunc.h"
#include "pecmd.h" 		// �ꉞinclude���� (NULLTEXT�g�p)

TCHAR uNxPath[11], ulPath[_MAX_DIR + _MAX_FNAME + _MAX_EXT + 1], ufFilePath[MAX_PATH + 1], uaFilePath1[MAX_PATH + 1], uaFilePath2[MAX_PATH + 1], uaFilePath3[MAX_PATH + 1], uaFilePath4[MAX_PATH + 1], uaFilePath5[MAX_PATH + 1], uaFilePath6[MAX_PATH + 1], uaFilePath7[MAX_PATH + 1], uaFilePath8[MAX_PATH + 1], uaFilePath9[MAX_PATH + 1];

// �t�@�C���̑��݊m�F
BOOL FileExist(LPCTSTR lpFileName)
{
	BOOL bResult;
	WIN32_FIND_DATA wfd;
	
	HANDLE hFile = FindFirstFile(lpFileName, &wfd);
	
	if (hFile == INVALID_HANDLE_VALUE) {
		bResult = FALSE;
	} else {
		bResult = TRUE;
	}
	
	FindClose(hFile);
	
	return bResult;
}

// �t�@�C���I���_�C�A���O
BOOL FileOpenDialog(HWND hWnd, LPCTSTR szFilter, LPCTSTR szDefExt, TCHAR *szFilePath)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));		// �ŏ��Ƀ[���N���A���Ă���
	ofn.lStructSize = sizeof(ofn);				// �\���̂̃T�C�Y
	ofn.hwndOwner = hWnd;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
	ofn.lpstrFilter = szFilter; 				// �t�@�C���̎��
	ofn.lpstrFile = szFilePath;					// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
	ofn.nMaxFile = MAX_PATH;			// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;	// �t���O�w��
	ofn.lpstrDefExt = szDefExt;					// �f�t�H���g�̃t�@�C���̎��
	
	if (!GetOpenFileName(&ofn)) {
		return FALSE;
	} else {
		return TRUE;
	}
}

// �t�@�C�����s (ShellExecute�֐��̈����Ɠ���)
HINSTANCE Execute(HWND hWnd, LPCTSTR lpVerb, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, int nShow)
{
	SHELLEXECUTEINFO sei; 
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));		// �[���N���A
	sei.cbSize = sizeof(SHELLEXECUTEINFO);			// �\���̃T�C�Y
	sei.fMask = SEE_MASK_INVOKEIDLIST;				//
	sei.hwnd = hWnd;								// �E�B���h�E�n���h��
	sei.lpVerb = lpVerb;							// ����
	sei.lpFile = lpFile;							// �t�@�C����
	sei.lpParameters = lpParameters;				// �p�����[�^�[
	sei.lpDirectory = lpDirectory;					// Current Directory
	sei.nShow = nShow;								// �\�����
	ShellExecuteEx(&sei);

	return sei.hInstApp;
}

// ini�t�@�C������f�[�^�̓ǂݏo�� (�G���[�����Ȃ�)
void LoadIniData(void)
{
	// �L�[����f�[�^�̓ǂݏo��
	GetPrivateProfileString(TEXT("pecmd"), TEXT("usbpath"), NULLTEXT, uNxPath, sizeof(uNxPath), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("usblaunch"), NULLTEXT, ulPath, sizeof(ulPath), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userfiler"), NULLTEXT, ufFilePath, sizeof(ufFilePath), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps1"), NULLTEXT, uaFilePath1, sizeof(uaFilePath1), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps2"), NULLTEXT, uaFilePath2, sizeof(uaFilePath2), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps3"), NULLTEXT, uaFilePath3, sizeof(uaFilePath3), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps4"), NULLTEXT, uaFilePath4, sizeof(uaFilePath4), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps5"), NULLTEXT, uaFilePath5, sizeof(uaFilePath5), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps6"), NULLTEXT, uaFilePath6, sizeof(uaFilePath6), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps7"), NULLTEXT, uaFilePath7, sizeof(uaFilePath7), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps8"), NULLTEXT, uaFilePath8, sizeof(uaFilePath8), INI);
	GetPrivateProfileString(TEXT("pecmd"), TEXT("userapps9"), NULLTEXT, uaFilePath9, sizeof(uaFilePath9), INI);

	// ��������
	CharLower(uNxPath);
}

// �t�@�C���p�X����t�@�C���̖��O�݂̂����o��
int FilePathToFileName(LPCTSTR lpFilePath, TCHAR *lpFileName)
{
	TCHAR szDrive[_MAX_DRIVE+1];
	TCHAR szDir[_MAX_DIR+1];
	TCHAR szFname[_MAX_FNAME+1];
	TCHAR szExt[_MAX_EXT+1];
	
	// �p�X�𕪉�
	_tsplitpath_s(lpFilePath, szDrive, (UINT)sizeof(szDrive), szDir, (UINT)sizeof(szDir), szFname, (UINT)sizeof(szFname), szExt, (UINT)sizeof(szExt));
	
	// �t�@�C����(�g���q����)�𐶐�
	lstrcpy(lpFileName, szFname);
	lstrcat(lpFileName, szExt);
	
	// ��������
	CharLower(lpFileName);
	
	return 0;
	
}
