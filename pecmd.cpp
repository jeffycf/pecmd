// pecmd.cpp : Launcher PEのメイン

/*
	WinPe-tchに付属のLauncher PEを、64BitPE上でも使用できるようにC++で一から書きました。
	
	たぶんバグだらけ
	
	英語はめちゃくちゃ

	Release でのビルドをお勧めします。(Release - Japanese を選択すると、日本語でビルドします)
*/

#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include "controls.h"
#include "FileFunc.h"
#include "pecmd.h"

// ウィンドウクラス名
#define PECMD_WNDCLASSNAME TEXT("pecmd")

ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HWND hParent;
// hPrevInstance は使用しない。もう使われていないから。
// _tWinMain を WinMain に絶対に変更しないこと。(コマンドライン文字列が正常に受け取れなくなる)
int WINAPI _tWinMain(HINSTANCE hCurInst, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	
	// COMの初期化 一応入れる
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
#ifdef PECMD_JAPANESE
		MessageBox(NULL, TEXT("COMの初期化に失敗しました。\n\nアプリケーションを終了します。"), TEXT("アプリケーションエラー"), MB_OK | MB_ICONERROR);
#else
		MessageBox(NULL, TEXT("Failed to initialize COM.\n\nExit this application."), TEXT("Application Error"), MB_OK | MB_ICONERROR);
#endif
		return 2;
	}
	
	// OLEの初期化 一応 (実際のところ、入れなくてもいいかわからない)
	OleInitialize(NULL);
	
	// ファイル選択ダイアログ
	if (!lstrcmpi(lpszCmdLine, TEXT("filedialog"))) {
		TCHAR szFilePath[MAX_PATH + 1] = {0};
#ifdef PECMD_JAPANESE
		FileOpenDialog(NULL, TEXT("すべてのファイル (*.*)\0*.*\0\0"), NULLTEXT, szFilePath);
#else
		FileOpenDialog(NULL, TEXT("All Files (*.*)\0*.*\0\0"), NULLTEXT, szFilePath);
#endif
		OleUninitialize();
		CoUninitialize();
		return 0;
	}
	
	// ウィンドウクラスの登録
	if (!InitApp(hCurInst)) {
#ifdef PECMD_JAPANESE
		MessageBox(NULL, TEXT("ウィンドウクラスの登録に失敗しました。\n\nアプリケーションを終了します。"), TEXT("アプリケーションエラー"), MB_OK | MB_ICONERROR);
#else
		MessageBox(NULL, TEXT("Failed to register the window class.\n\nExit this application."), TEXT("Application Error"), MB_OK | MB_ICONERROR);
#endif
		return 2;
	}
	
	// ウィンドウの作成
	if (!InitInstance(hCurInst, nCmdShow)) {
#ifdef PECMD_JAPANESE
		MessageBox(NULL, TEXT("ウィンドウの作成に失敗しました。\n\nアプリケーションを終了します。"), TEXT("アプリケーションエラー"), MB_OK | MB_ICONERROR);
#else
		MessageBox(NULL, TEXT("Failed to create the window.\n\nExit this application."), TEXT("Application Error"), MB_OK | MB_ICONERROR);
#endif
		return 2;
	}

	if (FileExist(TEXT("startnet2.cmd")) == TRUE)
		Execute(NULL, NULL, TEXT("startnet2.cmd"), NULL, NULL, SW_SHOWMINIMIZED);
	
	// メッセージを取得
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
#ifdef PECMD_JAPANESE
			MessageBox(msg.hwnd, TEXT("メッセージ処理エラーが発生しました。"), TEXT("アプリケーションエラー"), MB_OK | MB_ICONERROR | MB_TOPMOST);
#else
			MessageBox(msg.hwnd, TEXT("Message processing error has occurred."), TEXT("Application Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
#endif
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
						HWND hFocus = GetFocus(), hFocus_Parent = (HWND)GetWindowLongPtr(hFocus, GWLP_HWNDPARENT);
						if ((hFocus != NULL) && (hFocus_Parent != msg.hwnd))
							SendMessage((HWND)GetWindowLongPtr(hFocus, GWLP_HWNDPARENT), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hFocus, GWLP_ID), BN_CLICKED), (LPARAM)hFocus);
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
#ifdef PECMD_JAPANESE
			TEXT(" PEのランチャー - シェル"),
#else
			TEXT(" Launcher PE - Shell"),
#endif
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
	hParent = hWnd;
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static LPCTSTR lpszMesStr1 = TEXT("or"), lpszMesStr2 = TEXT("x");
#ifdef PECMD_JAPANESE
	static LPCTSTR lpszMesStr3 = TEXT("プログラムの切り替え : オルト + タブ");
#else
	static LPCTSTR lpszMesStr3 = TEXT("Switch Programs : Alt + Tab");
#endif
	static HFONT hFont; // フォントハンドル
	static TCHAR szSystemDir[MAX_PATH + 1];

	switch (msg)
	{
		case WM_CREATE:
			{
				// コントロールクラス
				ctls = new Controls;
				
				// INIファイルの情報を読み込み
				LoadIniData();
				
				// ボタンなどのコントロールを作成
				if (ctls->CreateControls(hWnd, ((LPCREATESTRUCT)lParam)->hInstance) != 0) {
					MessageBox(NULL, TEXT("Fatal Error."), TEXT("Error"), MB_OK | MB_ICONERROR);
					return -1;
				}

				// フォントハンドル取得
				hFont = CreateFont(
					-12, 0, 0, 0, FW_NORMAL,
					FALSE, FALSE, FALSE,
#ifdef PECMD_JAPANESE
					SHIFTJIS_CHARSET,
#else
					DEFAULT_CHARSET,
#endif
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, NULLTEXT
				);

				// システムDirectoryを取得
				GetSystemDirectory(szSystemDir, MAX_PATH + 1);
			}
			break;
		
		case WM_PAINT:
			{
				// ペイント処理
				hdc = BeginPaint(hWnd, &ps);
				SelectObject(hdc, hFont); // フォントを選択
				if (ctls->bSetRes == TRUE) {
					// SetResが使用できる場合
					TextOut(hdc, 138, 265, lpszMesStr1, lstrlen(lpszMesStr1));
					TextOut(hdc, 190, 265, lpszMesStr2, lstrlen(lpszMesStr2));
				}
				TextOut(hdc, 15, 300, lpszMesStr3, lstrlen(lpszMesStr3));
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
				
				case IDC_OTHERPARTITIONTOOL:
					// 他のパーティーションツール
					Execute(NULL, NULL, TEXT("X:\\Program Files\\otherptt.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_COMMANDPROMPT:
					// Command Prompt
					// Current Directoryは、システム固定で(いろいろと狂うと個人的に気持ち悪いしめんどくさい)
					Execute(NULL, NULL, TEXT("cmd.exe"), NULL, szSystemDir, SW_SHOWDEFAULT);
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
#ifdef PECMD_JAPANESE
						if (FileOpenDialog(NULL, TEXT("ドライバ情報ファイル (*.inf)\0*.inf\0すべてのファイル (*.*)\0*.*\0\0"), NULLTEXT, szFilePath) == TRUE) {
#else
						if (FileOpenDialog(NULL, TEXT("Driver Information File (*.inf)\0*.inf\0All Files (*.*)\0*.*\0\0"), NULLTEXT, szFilePath) == TRUE) {
#endif
							if (FileExist(szFilePath) == TRUE) {
								// Driver Information Fileが存在する場合
								wsprintf(szDrvLoadCmd, TEXT("\"%s\""), szFilePath);
								Execute(NULL, NULL, TEXT("drvload.exe"), szDrvLoadCmd, NULL, SW_SHOWDEFAULT);
							} else {
								// Driver Information Fileが存在しない場合
#ifdef PECMD_JAPANESE
								MessageBox(NULL, TEXT("ファイルが見つかりません。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
								MessageBox(NULL, TEXT("File not found."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
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
					Execute(NULL, NULL, TEXT("X:\\sources\\recovery\\recenv.exe -notshell"), NULL, NULL, SW_SHOWDEFAULT);
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
						for(int i = 0; i < 26; i++)
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
					Execute(NULL, NULL, TEXT("setres.exe"), TEXT("h800 v600 b32"), NULL, SW_SHOWDEFAULT);
					break;
					
				case IDC_RESXGA:
					// XGA
					Execute(NULL, NULL, TEXT("setres.exe"), TEXT("h1024 v768 b32"), NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_RESSXGA:
					// SXGA
					Execute(NULL, NULL, TEXT("setres.exe"), TEXT("h1280 v1024 b32"), NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_RESHD:
					// HD
					Execute(NULL, NULL, TEXT("setres.exe"), TEXT("h1920 v1080 b32"), NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_RESCHANGE:
					// Change Resolution
					{
						TCHAR res1[6], res2[6], szResCmd[15];
						ctls->GetResText(res1, 1);
						ctls->GetResText(res2, 2);
						wsprintf(szResCmd, TEXT("h%s v%s b32"), res1, res2);
						Execute(NULL, NULL, TEXT("setres.exe"), szResCmd, NULL, SW_SHOWDEFAULT);
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
#ifdef PECMD_JAPANESE
				if (MessageBox(hWnd, TEXT("シャットダウンしますか ?"), TEXT("シェルを終了します"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
#else
				if (MessageBox(hWnd, TEXT("Shutdown now ?"), TEXT("Shell-Application will exit"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
#endif
					// wpeutl.exeを実行
					if ((int)Execute(NULL, NULL, TEXT("wpeutil.exe"), TEXT("shutdown"), NULL, SW_SHOWDEFAULT) < 33) {
						// wpeutil.exeの起動に失敗 通常はこの時点でシャットダウンされている
#ifdef PECMD_JAPANESE
						MessageBox(hWnd, TEXT("シャットダウンに失敗しました。"), TEXT("エラー"), MB_OK | MB_ICONERROR);
#else
						MessageBox(hWnd, TEXT("Failed to shutdown."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
					} else
						break; // wpeutil.exeの起動に成功したら、あとはPEが自動で電源を切ってくれる。
							   // (このままこのアプリケーションが終了すると、シャットダウンのはずが再起動になってしまうかもしれないので。)
					
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
			/*
		case WM_KEYDOWN:
			if (wParam == VK_RETURN)
			{
				MessageBox(NULL, TEXT("Enter"), TEXT("Test"), MB_OK);
				HWND hFocus;
				LONG_PTR id;
				hFocus = GetFocus();
				if ((hFocus != NULL) && (hFocus != hWnd)) {
					id = GetWindowLongPtr(hFocus, GWLP_ID);
					TCHAR szBuf[256];
					wsprintf(szBuf, TEXT("%d"), id);
					MessageBox(NULL, szBuf, TEXT("Test"), MB_OK);
					SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hFocus, GWLP_ID), BN_CLICKED), (LPARAM)hFocus);
				}
			}
			break;
		*/
		default:
			return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
	
	return 0;
}

