// pecmd.cpp : Launcher PEのメイン

/*
	Launcher PE
	
	WinPe-tchに付属のLauncher PEを、64BitPE上でも使用できるようにC++で一から書きました。
	
	英語はめちゃくちゃ

	Release でのビルドをお勧めします。

	そのうち、言語ファイルを個別に用意して、対応できるようにしたい。

	by あける
*/

#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include "controls.h"
#include "FileFunc.h"
#include "pecmd.h"
#include "res.h"
#include "lang.h"

// ウィンドウクラス名
#define PECMD_WNDCLASSNAME TEXT("pecmd")

ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// hPrevInstance は使用しない。もう使われていないから。
// _tWinMain を WinMain に絶対に変更しないこと。(コマンドライン文字列が正常に受け取れなくなる)
int WINAPI _tWinMain(HINSTANCE hCurInst, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	int filedlg_flag = 0;
	
	if (!lstrcmpi(lpszCmdLine, TEXT("filedialog")))
		filedlg_flag = 1;
	
	// 言語ファイルの読み込み
	LoadLangFile(filedlg_flag);
	
	// COMの初期化 一応入れる
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		MessageBox(NULL, langStr.szErrorCOM, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR);
		return 2;
	}
	
	// OLEの初期化 一応 (実際のところ、入れなくてもいいかわからない)
	OleInitialize(NULL);
	
	// ファイル選択ダイアログ
	if (filedlg_flag) {
		TCHAR szFilePath[MAX_PATH + 1] = {0};
		FileOpenDialog(NULL, langStr.szAllFiles, NULLTEXT, szFilePath);
		OleUninitialize();
		CoUninitialize();
		return 0;
	}
	
	// ウィンドウクラスの登録
	if (!InitApp(hCurInst)) {
		MessageBox(NULL, langStr.szErrorWndClass, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR);
		return 2;
	}
	
	// ウィンドウの作成
	if (!InitInstance(hCurInst, nCmdShow)) {
		MessageBox(NULL, langStr.szErrorCreateWindow, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR);
		return 2;
	}
	
	// startnet2.cmdが存在する場合は実行する
	if (FileExist(TEXT("startnet2.cmd")))
		Execute(NULL, NULL, TEXT("startnet2.cmd"), NULL, NULL, SW_SHOWMINIMIZED);
	
	// メッセージを取得
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			MessageBox(msg.hwnd, langStr.szErrorGetMsg, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR | MB_TOPMOST);
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			// ここでWM_KEYDOWNを処理 WndProc内だと一回しか来ないようなので
			if (msg.message == WM_KEYDOWN) {
				switch (msg.wParam)
				{
				case VK_TAB:
					{
						// Tabキーによるフォーカス移動
						int flag = 1;
						if (GetAsyncKeyState(VK_SHIFT)&0x8000)
							flag = -1;
						ctls->SetTabKeyFocus(flag);
					}
					break;
					
				case VK_RETURN:
					{
						// Enterキーを押した際、ボタンを押したときと同じような効果を出す
						HWND hFocus = GetFocus();
						if (hFocus != NULL)
							SendMessage(hFocus, BM_CLICK, 0, 0);
					}
					break;
				}
			}
		}
	}
	
	// 開放
	OleUninitialize();
	CoUninitialize();
	
	return (int)msg.wParam;
}

// ウィンドウクラスの登録
ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PECMD)); // LoadIconを使用しないと、ウィンドウにアイコンが出ない。
	wc.hCursor = (HCURSOR)LoadImage(
		NULL, MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR, 0, 0,
		LR_DEFAULTSIZE | LR_SHARED);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = PECMD_WNDCLASSNAME;
	wc.hIconSm = wc.hIcon;
	
	return (RegisterClassEx(&wc));
}

// ウィンドウの生成
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;
	RECT rect = {0, 0, 290, 360};
	
	//クライアント領域のサイズからウィンドウ全体のサイズを計算
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, FALSE, 0);
	
	//ウィンドウ作成
	hWnd = CreateWindowEx(0, PECMD_WNDCLASSNAME,
			langStr.szTitle,
			WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right-rect.left,
			rect.bottom-rect.top,
			NULL,
			NULL,
			hInst,
			NULL
	);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static LPCTSTR lpszMesStr1 = TEXT("or"), lpszMesStr2 = TEXT("x");
	static HFONT hFont; // フォントハンドル
	
	switch (msg)
	{
		case WM_CREATE:
			{
				// コントロールクラス
				ctls = new Controls;
				
				// システムディレクトリを取得
				GetSysDir();
				
				// INIファイルの情報を読み込み
				LoadIniData();
				
				// ボタンなどのコントロールを作成
				if (ctls->CreateControls(hWnd, ((LPCREATESTRUCT)lParam)->hInstance)) {
					MessageBox(NULL, TEXT("Fatal Error."), TEXT("Error"), MB_OK | MB_ICONERROR);
					return -1;
				}
				
				// フォントハンドル取得
				hFont = CreateFont(
					-12, 0, 0, 0, FW_NORMAL,
					FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, NULLTEXT
				);
			}
			break;
		
		case WM_PAINT:
			{
				// ペイント処理
				hdc = BeginPaint(hWnd, &ps);
				SelectObject(hdc, hFont); // フォントを選択
				TextOut(hdc, 138, 265, lpszMesStr1, lstrlen(lpszMesStr1));
				TextOut(hdc, 190, 265, lpszMesStr2, lstrlen(lpszMesStr2));
				TextOut(hdc, 15, 300, langStr.szSwitchPrograms, lstrlen(langStr.szSwitchPrograms));
				EndPaint(hWnd, &ps);
			}
			break;
		
		case WM_COMMAND:
			// コントロール類の処理 (外部アプリケーション起動時は、エラー判定なし。)
			// Execute関数の第二引数は、空でもいいのか？
			switch (LOWORD(wParam))
			{
				case IDC_FILER:
					// 自由に設定できるファイラー
					Execute(NULL, NULL, ufFilePath, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_QDIR:
					// Q-Dir
					Execute(NULL, NULL, TEXT("X:\\Program Files\\q-dir\\q-dir.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_7ZIP:
					// 7-Zip
					Execute(NULL, NULL, TEXT("X:\\Program Files\\7-Zip\\7zFM.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_FILEOPENDLG:
					// ファイルダイアログ コマンドライン引数の"filedialog"はファイルパスと一緒に
					Execute(NULL, NULL, TEXT("X:\\Windows\\System32\\pecmd.exe"), TEXT("filedialog"), NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_ACROBACKRECOVER:
					// Acronis Backup & Recovery 10
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\BackupAndRecovery\\trueimage_starter.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_ACROTRUEIMAGE2009:
					// Acronis True Image 2009以降
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImageHome\\trueimage_starter.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
					
				case IDC_ACROTRUEIMAGE11:
					// Acronis True Image 11
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage_starter.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_ACROTRUEIMAGE10OR9HOME:
					// Acronis True Image 10/9 Home
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_ACROTRUEIMAGELEORPERSONAL2:
					// Acronis True Image LE/Personal2
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage\\TrueImage.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_DISCWIZARD:
					// DiscWizard
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Seagate\\DiscWizard_starter.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_PARAGBACKRECOVER:
					// Paragon Backup & Recovery
					Execute(NULL, NULL, TEXT("X:\\Program Files\\PBR\\program\\launcher.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_EASEUSTODOBACKUP25PE:
					// EASEUS Todo Backup 2.5x (公式PE互換)
					Execute(NULL, NULL, TEXT("X:\\Program Files\\easeus\\tb2.0\\bin\\PELoader.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_EASEUSTODOBACKUP4002PE:
					// EaseUS Todo Backup 4.0.0.2 (公式PE互換)
					Execute(NULL, NULL, TEXT("X:\\Program Files\\easeus\\tb\\bin\\PELoader.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_EASEUSTODOBACKUP25WIN:
					// EASEUS Todo Backup 2.5x (Win版互換)
					Execute(NULL, NULL, TEXT("X:\\Program Files\\ETB\\bin\\EuWatch.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					Execute(NULL, NULL, TEXT("X:\\Program Files\\ETB\\bin\\todostart.cmd"), NULL, NULL, SW_SHOWDEFAULT);
					Execute(NULL, NULL, TEXT("X:\\Program Files\\ETB\\bin\\loader.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_AOMEIBACKUPPER:
					// AOMEI Backupper
					Execute(NULL, NULL, TEXT("X:\\Program Files\\AomeiBackupper\\Backupper.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_OTHERBACKUPTOOL:
					// 他のバックアップツール
					Execute(NULL, NULL, TEXT("X:\\Program Files\\otherbpt.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_EASEUSPARTITIONMASTER:
					// EaseUS Partition Master
					Execute(NULL, NULL, TEXT("X:\\Program Files\\EPM\\bin\\epm0.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_PARTITIONWIZARD:
					// Partition Wizard
					Execute(NULL, NULL, TEXT("X:\\Program Files\\MPW\\PartitionWizard.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_ACRODISKDIRECTOR11:
					// Acronis Disk Director 11
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\DiskDirector\\trueimage_starter.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_PARAGPARTITIONMANAGER:
					// Paragon Partition Manager
					Execute(NULL, NULL, TEXT("X:\\Program Files\\PPM\\program\\launcher.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_AOMEIPARTITIONASSISTANT:
					// AOMEI Partition Assistant
					Execute(NULL, NULL, TEXT("X:\\Program Files\\PA\\PartAssist.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_OTHERPARTITIONTOOL:
					// 他のパーティーションツール
					Execute(NULL, NULL, TEXT("X:\\Program Files\\otherptt.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_COMMANDPROMPT:
					// Command Prompt
					// Current Directoryは、システム固定で(いろいろと狂うと個人的に気持ち悪いしめんどくさい)
					Execute(NULL, NULL, TEXT("cmd.exe"), NULL, szSysDir, SW_SHOWDEFAULT);
					break;
					
				case IDC_NOTEPAD:
					// Notepad
					Execute(NULL, NULL, TEXT("notepad.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_REGISTRYEDITOR:
					// Registry Editor
					Execute(NULL, NULL, TEXT("regedit.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_DISKPART:
					// Diskpart
					Execute(NULL, NULL, TEXT("diskpart.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_LOADDRIVER:
					{
						// Load Driver
						TCHAR szFilePath[MAX_PATH + 1] = {0};
						TCHAR szDrvLoadCmd[MAX_PATH + 1];
						
						if (FileOpenDialog(NULL, langStr.szDrvLoadInfo, NULLTEXT, szFilePath)) {
							if (FileExist(szFilePath) == TRUE) {
								// Driver Information Fileが存在する場合
								wsprintf(szDrvLoadCmd, TEXT("\"%s\""), szFilePath);
								Execute(NULL, NULL, TEXT("drvload.exe"), szDrvLoadCmd, NULL, SW_SHOWDEFAULT);
							} else {
								// Driver Information Fileが存在しない場合
								MessageBox(hWnd, langStr.szErrorFileNotFound, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
							}
						}
					}
					break;
				
				case IDC_MSIME:
					// MS-IME
					Execute(NULL, NULL, TEXT("imecmd.cmd"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_WINDOWSRE:
					// Windows RE
					Execute(NULL, NULL, TEXT("X:\\sources\\recovery\\recenv.exe"), TEXT("-notshell"), NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_INSTALLWINDOWS:
					// Install Windows (NT6.x)
					Execute(NULL, NULL, TEXT("X:\\sources\\setup.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP1:
					// App1
					Execute(NULL, NULL, uaFilePath1, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP2:
					// App2
					Execute(NULL, NULL, uaFilePath2, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP3:
					// App3
					Execute(NULL, NULL, uaFilePath3, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP4:
					// App4
					Execute(NULL, NULL, uaFilePath4, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP5:
					// App5
					Execute(NULL, NULL, uaFilePath5, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP6:
					// App6
					Execute(NULL, NULL, uaFilePath6, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP7:
					// App7
					Execute(NULL, NULL, uaFilePath7, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP8:
					// App8
					Execute(NULL, NULL, uaFilePath8, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_APP9:
					// App9
					Execute(NULL, NULL, uaFilePath9, NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_PSTART:
					// Pstart or Custom USB Memory Launcher
					{
						TCHAR szLaunchPath[MAX_PATH + 1];
						for(int i = 1; i < 26; i++)
						{
							wsprintf(szLaunchPath, TEXT("%c:\\%s"), 'A'+i, ulPath);
							if (FileExist(szLaunchPath) == TRUE) {
								Execute(NULL, NULL, szLaunchPath, NULL, NULL, SW_SHOWDEFAULT);
								break;
							}
						}
					}
					break;
				
				case IDC_RESDEF:
					// ResDefault
					SetDisplayRes(hWnd, 800, 600, 32);
					break;
					
				case IDC_RESXGA:
					// XGA
					SetDisplayRes(hWnd, 1024, 768, 32);
					break;
				
				case IDC_RESSXGA:
					// SXGA
					SetDisplayRes(hWnd, 1280, 1024, 32);
					break;
				
				case IDC_RESHD:
					// HD
					SetDisplayRes(hWnd, 1920, 1080, 32);
					break;
				
				case IDC_RESCHANGE:
					// Change Resolution
					{
						TCHAR res1[6], res2[6];
						ctls->GetResText(res1, 1);
						ctls->GetResText(res2, 2);
						SetDisplayRes(hWnd, _tstoi(res1), _tstoi(res2), 32);
					}
					break;
				
				case IDC_REBOOTWINPE:
					// Reboot WinPE
					Execute(NULL, NULL, TEXT("wpeutil.exe"), TEXT("reboot"), NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_SHUTDOWNWINPE:
					// Shutdown WinPE
					Execute(NULL, NULL, TEXT("wpeutil.exe"), TEXT("shutdown"), NULL, SW_SHOWDEFAULT);
					break;
				
			}
			break;
		
		case WM_CLOSE:
			{
				// 終了処理
				if (MessageBox(hWnd, langStr.szShutdown, langStr.szShutdownTitle, MB_YESNO | MB_ICONQUESTION) == IDYES) {
					// wpeutl.exeを実行
					if ((int)Execute(NULL, NULL, TEXT("wpeutil.exe"), TEXT("shutdown"), NULL, SW_SHOWDEFAULT) < 33) {
						// wpeutil.exeの起動に失敗 通常はこの時点でシャットダウンされている
						MessageBox(hWnd, langStr.szErrorShutdown, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
					} else
						break; // wpeutil.exeの起動に成功したら、あとはPEが自動で電源を切ってくれる。
						       // (このままこのアプリケーションが終了すると、シャットダウンのはずが再起動になってしまうかもしれないので。)
					
					// ウィンドウを削除
					DestroyWindow(hWnd);
				}
			}
			break;
		
		case WM_DESTROY:
			DeleteObject(hFont); // フォントを削除
			delete ctls; // クラスを解放
			PostQuitMessage(0); // WM_QUITを送信
			break;
		
		case WM_DRAWITEM:
			// ボタンの描画 (BS_OWNERDRAWスタイルが適用されているボタン)
			ctls->DrawImageButtonOnOwnerDraw(((LPDRAWITEMSTRUCT)(lParam))->hwndItem, ((LPDRAWITEMSTRUCT)(lParam))->hDC, ((LPDRAWITEMSTRUCT)(lParam))->itemState);
			break;

		default:
			return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
	
	return 0;
}
