// graphic.cpp : グラフィック関連の関数を提供 (イメージボタン用)

#include <windows.h>
#include <tchar.h>

#include <objidl.h>
#include <olectl.h>

#include "graphic.h"
#include "FileFunc.h"
#include "pecmd.h"

#define HIMETRIC_INCH   2540

// 画像読み込み
// LoadPictureOnResource 描画先のデバイスコンテキストハンドル, リソース名, リソースタイプ
BOOL LoadPictureOnResource(HDC hdc_main, LPCTSTR res, LPCTSTR type)
{
	IStream *iStream=NULL;
	IPicture *iPicture;
	HRSRC hFind;
	HGLOBAL hLoad, hMem;
	DWORD nSize;
	HMODULE hInst = GetModuleHandle(NULL);
	HRESULT result;
	long Height, Width;
	int pic_x, pic_y;
	
	// リソースからデータの読み出し
	if (!(hFind = FindResource(hInst, res, type))) return FALSE;
	if (!(hLoad = LoadResource(hInst, hFind))) return FALSE;
	if (!(nSize = SizeofResource(hInst, hFind))) return FALSE;
	if (!(hMem = GlobalAlloc(GHND, nSize))) return FALSE;
	
	// データのコピー
	CopyMemory(GlobalLock(hMem), LockResource(hLoad), nSize);
	GlobalUnlock(hMem);
	
	result = CreateStreamOnHGlobal(hMem, TRUE, &iStream);
	if (!SUCCEEDED(result))
		return FALSE;
	
	// 画像読み込み
	result = OleLoadPicture(iStream, nSize, FALSE, IID_IPicture, (LPVOID*)&iPicture);
	
	iStream->Release();
	
	if ((!SUCCEEDED(result)) || (iPicture == NULL))
		return FALSE;
	
	// 画像のサイズ取得
	iPicture->get_Width(&Width);
	iPicture->get_Height(&Height);
	
	// 画像の大きさの単位を変更
	pic_x = MulDiv(Width, GetDeviceCaps(hdc_main, LOGPIXELSX), HIMETRIC_INCH);
	pic_y = MulDiv(Height, GetDeviceCaps(hdc_main, LOGPIXELSY), HIMETRIC_INCH);
	
	// 画像を転送
	iPicture->Render(hdc_main, 0, 0, pic_x, pic_y, 0, Height, Width, -Height, NULL);
	
	// 後始末
	iPicture->Release();
	GlobalFree(hMem);
	
	return TRUE;
}

// フォルダアイコンを描画
void DrawFolderImage(HDC hdc, int pos_y1, int pos_y2)
{
	SHFILEINFO shfinfo;
	
	ZeroMemory(&shfinfo, sizeof(SHFILEINFO)); // 一応ゼロクリア
	SHGetFileInfo(NULLTEXT, NULL, &shfinfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_SMALLICON);
	DrawIconEx(hdc, 2, pos_y1, shfinfo.hIcon, 16, 16, 0, NULL, DI_NORMAL); // 通常時のアイコン
	DrawIconEx(hdc, 2, pos_y2, shfinfo.hIcon, 16, 16, 0, NULL, DI_NORMAL); // マウスがボタン内にあるときのアイコン
	DestroyIcon(shfinfo.hIcon);
}

// ファイルアイコンを描画 (例外あり)
void DrawFileImage(HDC hdc, LPCTSTR szFile, int pos_x, int pos_y)
{
	TCHAR FileName[MAX_PATH + 1];
	
	// ファイルパスからファイル名を取得
	FilePathToFileName(szFile, FileName);

	// 転送
	if (!lstrcmp(FileName, TEXT("wpeutil_disablefirewall.cmd"))) {
		// 例外1
		BitBlt(hdc, pos_x-2, pos_y-2, 20, 20, hdc, 0, 0, SRCCOPY);
	} else if (!lstrcmp(FileName, TEXT("wpeutil_enablefirewall.cmd"))) {
		// 例外2
		BitBlt(hdc, pos_x-2, pos_y-2, 20, 20, hdc, 0, 20, SRCCOPY);
	} else {
		// 通常
		SHFILEINFO shfinfo;
		ZeroMemory(&shfinfo, sizeof(SHFILEINFO));
		SHGetFileInfo(szFile, NULL, &shfinfo, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON); // ファイルのアイコンを取得
		DrawIconEx(hdc, pos_x, pos_y, shfinfo.hIcon, 16, 16, 0, NULL, DI_NORMAL); // アイコンを描画
		DestroyIcon(shfinfo.hIcon); // アイコンハンドルを破棄 (これをしないと一部環境ではおかしくなるらしい。)
	}
}

// 塗りつぶし (hspのboxfとほぼ同じ)
void Boxfill(HDC hdc, int x1, int y1, int x2, int y2, HBRUSH hbr)
{
	// RECT構造体に値をセット
	RECT rect = {x1, y1, x2+1, y2+1};
	
	// 塗りつぶし
	FillRect(hdc, &rect, hbr);
}

