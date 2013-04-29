// res.cpp : �摜�𑜓x�̕ύX

#include <windows.h>
#include <tchar.h>

#include "FileFunc.h"
#include "res.h"
#include "lang.h"

void SetDisplayRes(HWND hParent, int Height, int Width, int Bits)
{
	if (FileExist(TEXT("setres.exe"))) {
		// SetRes���g�p���ĉ摜�𑜓x��ύX����ꍇ
		TCHAR szResCmd[16];
		wsprintf(szResCmd, TEXT("h%d v%d b%d"), Height, Width, Bits);
		Execute(NULL, NULL, TEXT("setres.exe"), szResCmd, NULL, SW_SHOWDEFAULT);
		return;
	} else {
		// ChangeDisplaySettings API ���g�p���ĉ摜�𑜓x��ύX����ꍇ
		DEVMODE devmode;
		
		// ���݂̉�ʂ̐ݒ���擾
		if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode)) {
			MessageBox(hParent, langStr.szResFailedGetDispInfo, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
			return;
		}
		
		// �ύX����ݒ�
		devmode.dmPelsWidth = Height;
		devmode.dmPelsHeight = Width;
		devmode.dmBitsPerPel = Bits;
		devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		
		// �ݒ��ύX
		switch (ChangeDisplaySettings(&devmode, CDS_UPDATEREGISTRY))
		{
			case DISP_CHANGE_SUCCESSFUL:
				break;
			
			case DISP_CHANGE_RESTART:
				MessageBox(hParent, langStr.szResRestart, langStr.szInformationTitle, MB_OK | MB_ICONINFORMATION);
				break;
			
			case DISP_CHANGE_BADFLAGS:
				MessageBox(hParent, langStr.szResBadFlags, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
				break;
			
			case DISP_CHANGE_BADPARAM:
				MessageBox(hParent, langStr.szResBadParam, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
				break;
			
			case DISP_CHANGE_FAILED:
				MessageBox(hParent, langStr.szResFailed, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
				break;
			
			case DISP_CHANGE_BADMODE:
				MessageBox(hParent, langStr.szResBadMode, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
				break;
			
			case DISP_CHANGE_NOTUPDATED:
				MessageBox(hParent, langStr.szResNotUpdated, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
				break;
			
		}
		
		return;
	}
}
