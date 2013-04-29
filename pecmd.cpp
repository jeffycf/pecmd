// pecmd.cpp : Launcher PE�̃��C��

/*
	Launcher PE
	
	WinPe-tch�ɕt����Launcher PE���A64BitPE��ł��g�p�ł���悤��C++�ňꂩ�珑���܂����B
	
	�p��͂߂��Ⴍ����

	Release �ł̃r���h�������߂��܂��B

	���̂����A����t�@�C�����ʂɗp�ӂ��āA�Ή��ł���悤�ɂ������B

	by ������
*/

#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include "controls.h"
#include "FileFunc.h"
#include "pecmd.h"
#include "res.h"
#include "lang.h"

// �E�B���h�E�N���X��
#define PECMD_WNDCLASSNAME TEXT("pecmd")

ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// hPrevInstance �͎g�p���Ȃ��B�����g���Ă��Ȃ�����B
// _tWinMain �� WinMain �ɐ�΂ɕύX���Ȃ����ƁB(�R�}���h���C�������񂪐���Ɏ󂯎��Ȃ��Ȃ�)
int WINAPI _tWinMain(HINSTANCE hCurInst, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	int filedlg_flag = 0;
	
	if (!lstrcmpi(lpszCmdLine, TEXT("filedialog")))
		filedlg_flag = 1;
	
	// ����t�@�C���̓ǂݍ���
	LoadLangFile(filedlg_flag);
	
	// COM�̏����� �ꉞ�����
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		MessageBox(NULL, langStr.szErrorCOM, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR);
		return 2;
	}
	
	// OLE�̏����� �ꉞ (���ۂ̂Ƃ���A����Ȃ��Ă��������킩��Ȃ�)
	OleInitialize(NULL);
	
	// �t�@�C���I���_�C�A���O
	if (filedlg_flag) {
		TCHAR szFilePath[MAX_PATH + 1] = {0};
		FileOpenDialog(NULL, langStr.szAllFiles, NULLTEXT, szFilePath);
		OleUninitialize();
		CoUninitialize();
		return 0;
	}
	
	// �E�B���h�E�N���X�̓o�^
	if (!InitApp(hCurInst)) {
		MessageBox(NULL, langStr.szErrorWndClass, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR);
		return 2;
	}
	
	// �E�B���h�E�̍쐬
	if (!InitInstance(hCurInst, nCmdShow)) {
		MessageBox(NULL, langStr.szErrorCreateWindow, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR);
		return 2;
	}
	
	// startnet2.cmd�����݂���ꍇ�͎��s����
	if (FileExist(TEXT("startnet2.cmd")))
		Execute(NULL, NULL, TEXT("startnet2.cmd"), NULL, NULL, SW_SHOWMINIMIZED);
	
	// ���b�Z�[�W���擾
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			MessageBox(msg.hwnd, langStr.szErrorGetMsg, langStr.szAppErrorTitle, MB_OK | MB_ICONERROR | MB_TOPMOST);
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
						HWND hFocus = GetFocus();
						if (hFocus != NULL)
							SendMessage(hFocus, BM_CLICK, 0, 0);
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
	static HFONT hFont; // �t�H���g�n���h��
	
	switch (msg)
	{
		case WM_CREATE:
			{
				// �R���g���[���N���X
				ctls = new Controls;
				
				// �V�X�e���f�B���N�g�����擾
				GetSysDir();
				
				// INI�t�@�C���̏���ǂݍ���
				LoadIniData();
				
				// �{�^���Ȃǂ̃R���g���[�����쐬
				if (ctls->CreateControls(hWnd, ((LPCREATESTRUCT)lParam)->hInstance)) {
					MessageBox(NULL, TEXT("Fatal Error."), TEXT("Error"), MB_OK | MB_ICONERROR);
					return -1;
				}
				
				// �t�H���g�n���h���擾
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
				// �y�C���g����
				hdc = BeginPaint(hWnd, &ps);
				SelectObject(hdc, hFont); // �t�H���g��I��
				TextOut(hdc, 138, 265, lpszMesStr1, lstrlen(lpszMesStr1));
				TextOut(hdc, 190, 265, lpszMesStr2, lstrlen(lpszMesStr2));
				TextOut(hdc, 15, 300, langStr.szSwitchPrograms, lstrlen(langStr.szSwitchPrograms));
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
								// Driver Information File�����݂���ꍇ
								wsprintf(szDrvLoadCmd, TEXT("\"%s\""), szFilePath);
								Execute(NULL, NULL, TEXT("drvload.exe"), szDrvLoadCmd, NULL, SW_SHOWDEFAULT);
							} else {
								// Driver Information File�����݂��Ȃ��ꍇ
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
				// �I������
				if (MessageBox(hWnd, langStr.szShutdown, langStr.szShutdownTitle, MB_YESNO | MB_ICONQUESTION) == IDYES) {
					// wpeutl.exe�����s
					if ((int)Execute(NULL, NULL, TEXT("wpeutil.exe"), TEXT("shutdown"), NULL, SW_SHOWDEFAULT) < 33) {
						// wpeutil.exe�̋N���Ɏ��s �ʏ�͂��̎��_�ŃV���b�g�_�E������Ă���
						MessageBox(hWnd, langStr.szErrorShutdown, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
					} else
						break; // wpeutil.exe�̋N���ɐ���������A���Ƃ�PE�������œd����؂��Ă����B
						       // (���̂܂܂��̃A�v���P�[�V�������I������ƁA�V���b�g�_�E���̂͂����ċN���ɂȂ��Ă��܂���������Ȃ��̂ŁB)
					
					// �E�B���h�E���폜
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

		default:
			return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
	
	return 0;
}
