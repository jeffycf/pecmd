// FileFunc.cpp : ファイル関連の関数を提供

#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#include "FileFunc.h"
#include "pecmd.h" 		// 一応includeする (NULLTEXT使用)

TCHAR uNxPath[11], ulPath[_MAX_DIR + _MAX_FNAME + _MAX_EXT + 1], ufFilePath[MAX_PATH + 1], uaFilePath1[MAX_PATH + 1], uaFilePath2[MAX_PATH + 1], uaFilePath3[MAX_PATH + 1], uaFilePath4[MAX_PATH + 1], uaFilePath5[MAX_PATH + 1], uaFilePath6[MAX_PATH + 1], uaFilePath7[MAX_PATH + 1], uaFilePath8[MAX_PATH + 1], uaFilePath9[MAX_PATH + 1];

// ファイルの存在確認
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

// ファイル選択ダイアログ
BOOL FileOpenDialog(HWND hWnd, LPCTSTR szFilter, LPCTSTR szDefExt, TCHAR *szFilePath)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));		// 最初にゼロクリアしておく
	ofn.lStructSize = sizeof(ofn);				// 構造体のサイズ
	ofn.hwndOwner = hWnd;						// コモンダイアログの親ウィンドウハンドル
	ofn.lpstrFilter = szFilter; 				// ファイルの種類
	ofn.lpstrFile = szFilePath;					// 選択されたファイル名(フルパス)を受け取る変数のアドレス
	ofn.nMaxFile = MAX_PATH;			// lpstrFileに指定した変数のサイズ
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;	// フラグ指定
	ofn.lpstrDefExt = szDefExt;					// デフォルトのファイルの種類
	
	if (!GetOpenFileName(&ofn)) {
		return FALSE;
	} else {
		return TRUE;
	}
}

// ファイル実行 (ShellExecute関数の引数と同じ)
HINSTANCE Execute(HWND hWnd, LPCTSTR lpVerb, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, int nShow)
{
	SHELLEXECUTEINFO sei; 
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));		// ゼロクリア
	sei.cbSize = sizeof(SHELLEXECUTEINFO);			// 構造体サイズ
	sei.fMask = SEE_MASK_INVOKEIDLIST;				//
	sei.hwnd = hWnd;								// ウィンドウハンドル
	sei.lpVerb = lpVerb;							// 動作
	sei.lpFile = lpFile;							// ファイル名
	sei.lpParameters = lpParameters;				// パラメーター
	sei.lpDirectory = lpDirectory;					// Current Directory
	sei.nShow = nShow;								// 表示状態
	ShellExecuteEx(&sei);

	return sei.hInstApp;
}

// iniファイルからデータの読み出し (エラー処理なし)
void LoadIniData(void)
{
	// キーからデータの読み出し
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

	// 小文字化
	CharLower(uNxPath);
}

// ファイルパスからファイルの名前のみを取り出す
int FilePathToFileName(LPCTSTR lpFilePath, TCHAR *lpFileName)
{
	TCHAR szDrive[_MAX_DRIVE+1];
	TCHAR szDir[_MAX_DIR+1];
	TCHAR szFname[_MAX_FNAME+1];
	TCHAR szExt[_MAX_EXT+1];
	
	// パスを分解
	_tsplitpath_s(lpFilePath, szDrive, (UINT)sizeof(szDrive), szDir, (UINT)sizeof(szDir), szFname, (UINT)sizeof(szFname), szExt, (UINT)sizeof(szExt));
	
	// ファイル名(拡張子込み)を生成
	lstrcpy(lpFileName, szFname);
	lstrcat(lpFileName, szExt);
	
	// 小文字化
	CharLower(lpFileName);
	
	return 0;
	
}
