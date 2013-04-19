// res.cpp : 画像解像度の変更

#include <windows.h>
#include <tchar.h>

#include "FileFunc.h"
#include "res.h"

void SetDisplayRes(HWND hParent, int Height, int Width, int Bits)
{
	if (FileExist(TEXT("setres.exe"))) {
		// SetResを使用して画像解像度を変更する場合
		TCHAR szResCmd[16];
		wsprintf(szResCmd, TEXT("h%d v%d b%d"), Height, Width, Bits);
		Execute(NULL, NULL, TEXT("setres.exe"), szResCmd, NULL, SW_SHOWDEFAULT);
		return;
	} else {
		// ChangeDisplaySettings API を使用して画像解像度を変更する場合
		DEVMODE devmode;
		
		// 現在の画面の設定を取得
		if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode)) {
#ifdef PECMD_JAPANESE
			MessageBox(hParent, TEXT("現在のディスプレイの情報の取得に失敗しました。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
			MessageBox(hParent, TEXT("Failed to get information of the current display."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
			return;
		}
		
		// 変更する設定
		devmode.dmPelsWidth = Height;
		devmode.dmPelsHeight = Width;
		devmode.dmBitsPerPel = Bits;
		devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		
		// 設定を変更
		switch (ChangeDisplaySettings(&devmode, CDS_UPDATEREGISTRY))
		{
			case DISP_CHANGE_SUCCESSFUL:
				break;

			case DISP_CHANGE_RESTART:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("解像度を有効にするには、コンピュータを再起動させる必要があります。"), TEXT("情報"), MB_OK | MB_ICONINFORMATION);
#else
				MessageBox(hParent, TEXT("To turn on the resolution, you may need to restart the computer."), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
#endif
				break;

			case DISP_CHANGE_BADFLAGS:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("渡されたフラグセットが無効です。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Set of flag passed is invalid."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_BADPARAM:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("渡されたパラメータが無効です。\nパラメータに無効なフラグまたはフラグの組み合わせが含まれている可能性があります。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Parameter passed is invalid. \nThere is a possibility that the combination of flag or flag contains an invalid parameter."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_FAILED:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("解像度の設定に失敗しました。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Failed to change the resolution."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_BADMODE:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("この解像度はサポートされていません。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("This resolution is not supported."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

			case DISP_CHANGE_NOTUPDATED:
#ifdef PECMD_JAPANESE
				MessageBox(hParent, TEXT("レジストリに設定を書き込めません。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
				MessageBox(hParent, TEXT("Cannot write to the registry settings."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				break;

		}
		
		return;
	}
}
