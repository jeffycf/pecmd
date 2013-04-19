// res.cpp : �摜�𑜓x�̕ύX

#include <windows.h>
#include <tchar.h>

#include "FileFunc.h"
#include "res.h"

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
#ifdef PECMD_JAPANESE
			MessageBox(hParent, TEXT("���݂̃f�B�X�v���C�̏��̎擾�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
			MessageBox(hParent, TEXT("Failed to get information of the current display."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
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
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("�𑜓x��L���ɂ���ɂ́A�R���s���[�^���ċN��������K�v������܂��B"), TEXT("���"), MB_OK | MB_ICONINFORMATION);
#else
				MessageBox(hParent, TEXT("To turn on the resolution, you may need to restart the computer."), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
#endif
				break;

			case DISP_CHANGE_BADFLAGS:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("�n���ꂽ�t���O�Z�b�g�������ł��B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Set of flag passed is invalid."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_BADPARAM:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("�n���ꂽ�p�����[�^�������ł��B\n�p�����[�^�ɖ����ȃt���O�܂��̓t���O�̑g�ݍ��킹���܂܂�Ă���\��������܂��B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Parameter passed is invalid. \nThere is a possibility that the combination of flag or flag contains an invalid parameter."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_FAILED:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("�𑜓x�̐ݒ�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Failed to change the resolution."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_BADMODE:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("���̉𑜓x�̓T�|�[�g����Ă��܂���B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("This resolution is not supported."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_NOTUPDATED:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("���W�X�g���ɐݒ���������߂܂���B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Cannot write to the registry settings."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

		}
		
		return;
	}
}
