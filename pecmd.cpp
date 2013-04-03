// pecmd.cpp : Launcher PE�̃��C��

/*
	WinPe-tch�ɕt����Launcher PE���A64BitPE��ł��g�p�ł���悤��C++�ňꂩ�珑���܂����B
	
	���Ԃ�o�O���炯
	
	�p��͂߂��Ⴍ����

	Release �ł̃r���h�������߂��܂��B(Release - Japanese ��I������ƁA���{��Ńr���h���܂�)
*/

#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include "controls.h"
#include "FileFunc.h"
#include "pecmd.h"

// �E�B���h�E�N���X��
#define PECMD_WNDCLASSNAME TEXT("pecmd")

ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HWND hParent;
// hPrevInstance �͎g�p���Ȃ��B�����g���Ă��Ȃ�����B
// _tWinMain �� WinMain �ɐ�΂ɕύX���Ȃ����ƁB(�R�}���h���C�������񂪐���Ɏ󂯎��Ȃ��Ȃ�)
int WINAPI _tWinMain(HINSTANCE hCurInst, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	
	// COM�̏����� �ꉞ�����
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
#ifdef PECMD_JAPANESE
		MessageBox(NULL, TEXT("COM�̏������Ɏ��s���܂����B\n\n�A�v���P�[�V�������I�����܂��B"), TEXT("�A�v���P�[�V�����G���["), MB_OK | MB_ICONERROR);
#else
		MessageBox(NULL, TEXT("Failed to initialize COM.\n\nExit this application."), TEXT("Application Error"), MB_OK | MB_ICONERROR);
#endif
		return 2;
	}
	
	// OLE�̏����� �ꉞ (���ۂ̂Ƃ���A����Ȃ��Ă��������킩��Ȃ�)
	OleInitialize(NULL);
	
	// �t�@�C���I���_�C�A���O
	if (!lstrcmpi(lpszCmdLine, TEXT("filedialog"))) {
		TCHAR szFilePath[MAX_PATH + 1] = {0};
#ifdef PECMD_JAPANESE
		FileOpenDialog(NULL, TEXT("���ׂẴt�@�C�� (*.*)\0*.*\0\0"), NULLTEXT, szFilePath);
#else
		FileOpenDialog(NULL, TEXT("All Files (*.*)\0*.*\0\0"), NULLTEXT, szFilePath);
#endif
		OleUninitialize();
		CoUninitialize();
		return 0;
	}
	
	// �E�B���h�E�N���X�̓o�^
	if (!InitApp(hCurInst)) {
#ifdef PECMD_JAPANESE
		MessageBox(NULL, TEXT("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B\n\n�A�v���P�[�V�������I�����܂��B"), TEXT("�A�v���P�[�V�����G���["), MB_OK | MB_ICONERROR);
#else
		MessageBox(NULL, TEXT("Failed to register the window class.\n\nExit this application."), TEXT("Application Error"), MB_OK | MB_ICONERROR);
#endif
		return 2;
	}
	
	// �E�B���h�E�̍쐬
	if (!InitInstance(hCurInst, nCmdShow)) {
#ifdef PECMD_JAPANESE
		MessageBox(NULL, TEXT("�E�B���h�E�̍쐬�Ɏ��s���܂����B\n\n�A�v���P�[�V�������I�����܂��B"), TEXT("�A�v���P�[�V�����G���["), MB_OK | MB_ICONERROR);
#else
		MessageBox(NULL, TEXT("Failed to create the window.\n\nExit this application."), TEXT("Application Error"), MB_OK | MB_ICONERROR);
#endif
		return 2;
	}

	if (FileExist(TEXT("startnet2.cmd")) == TRUE)
		Execute(NULL, NULL, TEXT("startnet2.cmd"), NULL, NULL, SW_SHOWMINIMIZED);
	
	// ���b�Z�[�W���擾
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
#ifdef PECMD_JAPANESE
			MessageBox(msg.hwnd, TEXT("���b�Z�[�W�����G���[���������܂����B"), TEXT("�A�v���P�[�V�����G���["), MB_OK | MB_ICONERROR | MB_TOPMOST);
#else
			MessageBox(msg.hwnd, TEXT("Message processing error has occurred."), TEXT("Application Error"), MB_OK | MB_ICONERROR | MB_TOPMOST);
#endif
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// ������WM_KEYDOWN������ WndProc�����ƈ�񂵂����Ȃ��悤�Ȃ̂�
			if (msg.message == WM_KEYDOWN) {
				switch (msg.wParam)
				{
				case VK_TAB:
					{
						// Tab�L�[�ɂ��t�H�[�J�X�ړ�
						int flag = 1;
						if (GetAsyncKeyState(VK_SHIFT)&0x8000)
							flag = -1;
						ctls->SetTabKeyFocus(flag);
					}
					break;
					
				case VK_RETURN:
					{
						// Enter�L�[���������ہA�{�^�����������Ƃ��Ɠ����悤�Ȍ��ʂ��o��
						HWND hFocus = GetFocus(), hFocus_Parent = (HWND)GetWindowLongPtr(hFocus, GWLP_HWNDPARENT);
						if ((hFocus != NULL) && (hFocus_Parent != msg.hwnd))
							SendMessage((HWND)GetWindowLongPtr(hFocus, GWLP_HWNDPARENT), WM_COMMAND, MAKEWPARAM(GetWindowLongPtr(hFocus, GWLP_ID), BN_CLICKED), (LPARAM)hFocus);
					}
					break;
				}
			}
		}
	}
	
	// �J��
	OleUninitialize();
	CoUninitialize();
	
	return (int)msg.wParam;
}

// �E�B���h�E�N���X�̓o�^
ATOM InitApp(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PECMD)); // LoadIcon���g�p���Ȃ��ƁA�E�B���h�E�ɃA�C�R�����o�Ȃ��B
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

// �E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;
	RECT rect = {0, 0, 290, 360};
	
	//�N���C�A���g�̈�̃T�C�Y����E�B���h�E�S�̂̃T�C�Y���v�Z
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, FALSE, 0);
	
	//�E�B���h�E�쐬
	hWnd = CreateWindowEx(0, PECMD_WNDCLASSNAME,
#ifdef PECMD_JAPANESE
			TEXT(" PE�̃����`���[ - �V�F��"),
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
	static LPCTSTR lpszMesStr3 = TEXT("�v���O�����̐؂�ւ� : �I���g + �^�u");
#else
	static LPCTSTR lpszMesStr3 = TEXT("Switch Programs : Alt + Tab");
#endif
	static HFONT hFont; // �t�H���g�n���h��
	static TCHAR szSystemDir[MAX_PATH + 1];

	switch (msg)
	{
		case WM_CREATE:
			{
				// �R���g���[���N���X
				ctls = new Controls;
				
				// INI�t�@�C���̏���ǂݍ���
				LoadIniData();
				
				// �{�^���Ȃǂ̃R���g���[�����쐬
				if (ctls->CreateControls(hWnd, ((LPCREATESTRUCT)lParam)->hInstance) != 0) {
					MessageBox(NULL, TEXT("Fatal Error."), TEXT("Error"), MB_OK | MB_ICONERROR);
					return -1;
				}

				// �t�H���g�n���h���擾
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

				// �V�X�e��Directory���擾
				GetSystemDirectory(szSystemDir, MAX_PATH + 1);
			}
			break;
		
		case WM_PAINT:
			{
				// �y�C���g����
				hdc = BeginPaint(hWnd, &ps);
				SelectObject(hdc, hFont); // �t�H���g��I��
				if (ctls->bSetRes == TRUE) {
					// SetRes���g�p�ł���ꍇ
					TextOut(hdc, 138, 265, lpszMesStr1, lstrlen(lpszMesStr1));
					TextOut(hdc, 190, 265, lpszMesStr2, lstrlen(lpszMesStr2));
				}
				TextOut(hdc, 15, 300, lpszMesStr3, lstrlen(lpszMesStr3));
				EndPaint(hWnd, &ps);
			}
			break;
		
		case WM_COMMAND:
			// �R���g���[���ނ̏��� (�O���A�v���P�[�V�����N�����́A�G���[����Ȃ��B)
			// Execute�֐��̑������́A��ł������̂��H
			switch (LOWORD(wParam))
			{
				case IDC_FILER:
					// ���R�ɐݒ�ł���t�@�C���[
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
					// �t�@�C���_�C�A���O �R�}���h���C��������"filedialog"�̓t�@�C���p�X�ƈꏏ��
					Execute(NULL, NULL, TEXT("X:\\Windows\\System32\\pecmd.exe"), TEXT("filedialog"), NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_ACROBACKRECOVER:
					// Acronis Backup & Recovery 10
					Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\BackupAndRecovery\\trueimage_starter.exe"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_ACROTRUEIMAGE2009:
					// Acronis True Image 2009�ȍ~
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
					// EASEUS Todo Backup 2.5x (����PE�݊�)
					Execute(NULL, NULL, TEXT("X:\\Program Files\\easeus\\tb2.0\\bin\\PELoader.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_EASEUSTODOBACKUP4002PE:
					// EaseUS Todo Backup 4.0.0.2 (����PE�݊�)
					Execute(NULL, NULL, TEXT("X:\\Program Files\\easeus\\tb\\bin\\PELoader.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_EASEUSTODOBACKUP25WIN:
					// EASEUS Todo Backup 2.5x (Win�Ō݊�)
					Execute(NULL, NULL, TEXT("X:\\Program Files\\ETB\\bin\\EuWatch.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					Execute(NULL, NULL, TEXT("X:\\Program Files\\ETB\\bin\\todostart.cmd"), NULL, NULL, SW_SHOWDEFAULT);
					Execute(NULL, NULL, TEXT("X:\\Program Files\\ETB\\bin\\loader.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_OTHERBACKUPTOOL:
					// ���̃o�b�N�A�b�v�c�[��
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
					// ���̃p�[�e�B�[�V�����c�[��
					Execute(NULL, NULL, TEXT("X:\\Program Files\\otherptt.lnk"), NULL, NULL, SW_SHOWDEFAULT);
					break;
				
				case IDC_COMMANDPROMPT:
					// Command Prompt
					// Current Directory�́A�V�X�e���Œ��(���낢��Ƌ����ƌl�I�ɋC�����������߂�ǂ�����)
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
						if (FileOpenDialog(NULL, TEXT("�h���C�o���t�@�C�� (*.inf)\0*.inf\0���ׂẴt�@�C�� (*.*)\0*.*\0\0"), NULLTEXT, szFilePath) == TRUE) {
#else
						if (FileOpenDialog(NULL, TEXT("Driver Information File (*.inf)\0*.inf\0All Files (*.*)\0*.*\0\0"), NULLTEXT, szFilePath) == TRUE) {
#endif
							if (FileExist(szFilePath) == TRUE) {
								// Driver Information File�����݂���ꍇ
								wsprintf(szDrvLoadCmd, TEXT("\"%s\""), szFilePath);
								Execute(NULL, NULL, TEXT("drvload.exe"), szDrvLoadCmd, NULL, SW_SHOWDEFAULT);
							} else {
								// Driver Information File�����݂��Ȃ��ꍇ
#ifdef PECMD_JAPANESE
								MessageBox(NULL, TEXT("�t�@�C����������܂���B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
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
				// �I������
#ifdef PECMD_JAPANESE
				if (MessageBox(hWnd, TEXT("�V���b�g�_�E�����܂��� ?"), TEXT("�V�F�����I�����܂�"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
#else
				if (MessageBox(hWnd, TEXT("Shutdown now ?"), TEXT("Shell-Application will exit"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
#endif
					// wpeutl.exe�����s
					if ((int)Execute(NULL, NULL, TEXT("wpeutil.exe"), TEXT("shutdown"), NULL, SW_SHOWDEFAULT) < 33) {
						// wpeutil.exe�̋N���Ɏ��s �ʏ�͂��̎��_�ŃV���b�g�_�E������Ă���
#ifdef PECMD_JAPANESE
						MessageBox(hWnd, TEXT("�V���b�g�_�E���Ɏ��s���܂����B"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
						MessageBox(hWnd, TEXT("Failed to shutdown."), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
					} else
						break; // wpeutil.exe�̋N���ɐ���������A���Ƃ�PE�������œd����؂��Ă����B
							   // (���̂܂܂��̃A�v���P�[�V�������I������ƁA�V���b�g�_�E���̂͂����ċN���ɂȂ��Ă��܂���������Ȃ��̂ŁB)
					
					DestroyWindow(hWnd);
				}
			}
			break;
		
		case WM_DESTROY:
			DeleteObject(hFont); // �t�H���g���폜
			delete ctls; // �N���X�����
			PostQuitMessage(0); // WM_QUIT�𑗐M
			break;
		
		case WM_DRAWITEM:
			// �{�^���̕`�� (BS_OWNERDRAW�X�^�C�����K�p����Ă���{�^��)
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

