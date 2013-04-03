// controls.cpp : Launcher PE�̃{�^���Ȃǂ̃E�B���h�E�I�u�W�F�N�g���쐬
//              : �C���[�W�{�^���̊Ǘ����s��

#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include "controls.h"
#include "FileFunc.h"
#include "graphic.h"
#include "pecmd.h"

// HDC�̉���
#define HDC_MEMID1 0
#define HDC_MEMID2 1
#define HDC_MEMID3 2
#define HDC_MEMID4 3
#define HDC_MEMID5 4

static WNDPROC DefButtonProc;

Controls *ctls;

LRESULT CALLBACK ImgButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_MOUSEMOVE:
			ctls->DrawImageButtonOnMouseEvent(hWnd, 1);
			break;
		
		case WM_MOUSELEAVE:
			ctls->DrawImageButtonOnMouseEvent(hWnd, 0);
			break;
		
	}
	return CallWindowProc(DefButtonProc, hWnd, msg ,wParam ,lParam);
}

Controls::Controls(void)
{
	// GUI�t�H���g���擾
	Controls::hGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

Controls::~Controls(void)
{
	// GUI�t�H���g���폜
	DeleteObject(hGUIFont);
	
	// �C���[�W�{�^�������폜
	free(controlinfo);
	
	// �f�o�C�X�R���e�L�X�g�n���h����j��
	if (hdc_mem[0]) DeleteDC(hdc_mem[0]);
	if (hdc_mem[1]) DeleteDC(hdc_mem[1]);
	if (hdc_mem[2]) DeleteDC(hdc_mem[2]);
	if (hdc_mem[3]) DeleteDC(hdc_mem[3]);
	if (hdc_mem[4]) DeleteDC(hdc_mem[4]);
}

// �{�^���̍쐬�ƃC���[�W�{�^���� (��������)
int Controls::CreateControls(HWND hWnd, HINSTANCE hInst)
{
	int filer = 0, backuptool = 0, partitiontool = 0;
	UINT wintools = 0L, users_app = 0L;

	// �{�^���̃n���h��
	HWND hFilerButton, hQDirButton, h7ZipButton, hFileOpenButton, hAcroBackRecoverButton, hAcroTrueImage2009Button, hAcroTrueImage11Button, hAcroTrueImage10or9HomeButton, hAcroTrueImageLEorPersonal2Button, hDiscWizardButton, hParagonBackupAndRecoveryButton, hEaseUSTodoBackup25PEButton, hEaseUSTodoBackup4002PEButton, hEaseUSTodoBackup25WinButton, hOtherBackupToolButton, hEaseUSPartitionMasterButton, hPartitionWizardButton, hAcronisDiskDirectorButton, hParagonPartitionManagerButton, hOtherPartitionToolButton, hCommandPromptButton, hNotepadButton, hRegistryEditorButton, hDiskpartButton, hLoadDriverButton, hMSIMEButton, hWindowsREButton, hInstallWindowsButton, hApp1Button, hApp2Button, hApp3Button, hApp4Button, hApp5Button, hApp6Button, hApp7Button, hApp8Button, hApp9Button, hPStartButton, hResDefaultButton, hResXGAButton, hResSXGAButton, hResHDButton, hResChangeButton, hRebootButton, hShutdownButton;

	controls_all = 0;
	
	/*---------------------------------- ���������ʂ̃{�^�����쐬 ----------------------------------*/
	
	// �摜�{�^���ɂ���Ƃ��̓{�^���Ƀt�H���g�ݒ肵�Ȃ��̂�(�Ƃ��������Ȃ��Ă������̂�)�AWM_SETFONT�̓{�^���ɂ͓K�p���Ȃ�
	
	// �t�@�C���[
	if (FileExist(ufFilePath) == TRUE) {
		// ���[�U�[�����R�Ɏw��ł���t�@�C���[
		hFilerButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 20, 180, 20,
			hWnd, (HMENU)IDC_FILER, hInst, NULL
		);
		controls_all++;
		filer = 1;
	} else if (FileExist(TEXT("X:\\Program Files\\q-dir\\q-dir.exe")) == TRUE) {
		// Q-Dir�����݂���ꍇ
		hQDirButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 20, 180, 20,
			hWnd, (HMENU)IDC_QDIR, hInst, NULL
		);
		controls_all++;
		filer = 2;
	} else if (FileExist(TEXT("X:\\Program Files\\7-Zip\\7zFM.exe")) == TRUE) {
		// Q-Dir�����݂��Ȃ� & 7-Zip�����݂���ꍇ
		h7ZipButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 20, 180, 20,
			hWnd, (HMENU)IDC_7ZIP, hInst, NULL
		);
		controls_all++;
		filer = 3;
	}
	
	// �t�@�C���I�[�v���_�C�A���O, File Open Dialog
	hFileOpenButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		213, 20, 20, 20,
		hWnd, (HMENU)IDC_FILEOPENDLG, hInst, NULL
	);
	controls_all++;
	
	// �o�b�N�A�b�v�c�[��, Backup Tools
	if (FileExist(TEXT("X:\\Program Files\\Acronis\\BackupAndRecovery\\trueimage_starter.exe")) == TRUE) {
		// Acronis Backup & Recovery 10
		hAcroBackRecoverButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_ACROBACKRECOVER, hInst, NULL
		);
		controls_all++;
		backuptool = 1;
	} else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImageHome\\trueimage_starter.exe")) == TRUE) {
		// TrueImage2009�ȍ~
		hAcroTrueImage2009Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_ACROTRUEIMAGE2009, hInst, NULL
		);
		controls_all++;
		backuptool = 2;
	} else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImage_starter.exe")) == TRUE) {
		// TrueImage11
		hAcroTrueImage11Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_ACROTRUEIMAGE11, hInst, NULL
		);
		controls_all++;
		backuptool = 3;
	} else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImage.exe")) == TRUE) {
		// TrueImage10/9Home
		hAcroTrueImage10or9HomeButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_ACROTRUEIMAGE10OR9HOME, hInst, NULL
		);
		controls_all++;
		backuptool = 4;
	} else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImage\\TrueImage.exe")) == TRUE) {
		// TrueImageLE/Personal2
		Execute(NULL, NULL, TEXT("reg.exe"), TEXT("add HKCU\\Software\\Acronis\\TrueImage\\DontShow /v Updates /t REG_SZ /d 1 /f"), NULL, SW_SHOWMINIMIZED); //�ŏ�����ԂŎ��s
		Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage\\schedhlp.exe"), NULL, NULL, SW_SHOWDEFAULT);
		Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage\\TimounterMonitor.exe"), NULL, NULL, SW_SHOWDEFAULT);
		Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage\\TrueImageMonitor.exe"), NULL, NULL, SW_SHOWDEFAULT);
		hAcroTrueImageLEorPersonal2Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_ACROTRUEIMAGELEORPERSONAL2, hInst, NULL
		);
		controls_all++;
		backuptool = 5;
	} else if (FileExist(TEXT("X:\\Program Files\\Seagate\\DiscWizard_starter.exe")) == TRUE) {
		// DiscWizard
		hDiscWizardButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_DISCWIZARD, hInst, NULL
		);
		controls_all++;
		backuptool = 6;
	} else if (FileExist(TEXT("X:\\Program Files\\PBR\\program\\launcher.lnk")) == TRUE) {
		// Paragon Backup & Recovery
		hParagonBackupAndRecoveryButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_PARAGBACKRECOVER, hInst, NULL
		);
		controls_all++;
		backuptool = 7;
	} else if (FileExist(TEXT("X:\\Program Files\\easeus\\tb2.0\\bin\\PELoader.lnk")) == TRUE) {
		// EASEUS Todo Backup 2.5x (����PE�݊�)
		hEaseUSTodoBackup25PEButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_EASEUSTODOBACKUP25PE, hInst, NULL
		);
		controls_all++;
		backuptool = 8;
	} else if (FileExist(TEXT("X:\\Program Files\\easeus\\tb\\bin\\PELoader.lnk")) == TRUE) {
		// EaseUS Todo Backup 4.0.0.2 (����PE�݊�)
		hEaseUSTodoBackup4002PEButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_EASEUSTODOBACKUP4002PE, hInst, NULL
		);
		controls_all++;
		backuptool = 9;
	} else if (FileExist(TEXT("X:\\Program Files\\ETB\\bin\\loader.lnk")) == TRUE) {
		// EASEUS Todo Backup 2.5x (Win�Ō݊�)
		hEaseUSTodoBackup25WinButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_EASEUSTODOBACKUP25WIN, hInst, NULL
		);
		controls_all++;
		backuptool = 10;
	} else if (FileExist(TEXT("X:\\Program Files\\otherbpt.lnk")) == TRUE) {
		// ���̃o�b�N�A�b�v�c�[��
		hOtherBackupToolButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_OTHERBACKUPTOOL, hInst, NULL
		);
		controls_all++;
		backuptool = 11;
	}
	
	// �p�[�e�B�V�����c�[��
	if (FileExist(TEXT("X:\\Program Files\\EPM\\bin\\epm0.lnk")) == TRUE) {
		// EaseUS Partition Master
		hEaseUSPartitionMasterButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_EASEUSPARTITIONMASTER, hInst, NULL
		);
		controls_all++;
		partitiontool = 1;
	} else if (FileExist(TEXT("X:\\Program Files\\MPW\\PartitionWizard.lnk")) == TRUE) {
		// Partition Wizard
		hPartitionWizardButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_PARTITIONWIZARD, hInst, NULL
		);
		controls_all++;
		partitiontool = 2;
	} else if (FileExist(TEXT("X:\\Program Files\\Acronis\\DiskDirector\\trueimage_starter.exe")) == TRUE) {
		// Acronis Disk Director
		hAcronisDiskDirectorButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_ACRODISKDIRECTOR11, hInst, NULL
		);
		controls_all++;
		partitiontool = 3;
	} else if (FileExist(TEXT("X:\\Program Files\\PPM\\program\\launcher.lnk")) == TRUE) {
		// Paragon Partition Manager
		hParagonPartitionManagerButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_PARAGPARTITIONMANAGER, hInst, NULL
		);
		controls_all++;
		partitiontool = 4;
	} else if (FileExist(TEXT("X:\\Program Files\\otherptt.lnk")) == TRUE) {
		// ���̃p�[�e�B�V�����c�[��
		hOtherPartitionToolButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			25, 50, 210, 20,
			hWnd, (HMENU)IDC_OTHERPARTITIONTOOL, hInst, NULL
		);
		controls_all++;
		partitiontool = 5;
	}
	
	// Windows Tools
	
	// Command Prompt (�R�}���h�v�����v�g)
	hCommandPromptButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 130, 130, 20,
		hWnd, (HMENU)IDC_COMMANDPROMPT, hInst, NULL
	);
	controls_all++;
	
	// Notepad (������)
	hNotepadButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		145, 130, 130, 20,
		hWnd, (HMENU)IDC_NOTEPAD, hInst, NULL
	);
	controls_all++;
	
	// Registry Editor (���W�X�g���G�f�B�^�[)
	hRegistryEditorButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 155, 130, 20,
		hWnd, (HMENU)IDC_REGISTRYEDITOR, hInst, NULL
	);
	controls_all++;
	
	// Diskpart
	hDiskpartButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		145, 155, 130, 20,
		hWnd, (HMENU)IDC_DISKPART, hInst, NULL
	);
	controls_all++;
	
	// Load Driver (�h���C�o�ǂݍ���)
	hLoadDriverButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 180, 130, 20,
		hWnd, (HMENU)IDC_LOADDRIVER, hInst, NULL
	);
	controls_all++;
	
	// MS-IME (�t�@�C�����݊m�F�����܂��������킩��Ȃ�
	if (FileExist(TEXT("imecmd.cmd")) == TRUE) {
		hMSIMEButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			145, 180, 130, 20,
			hWnd, (HMENU)IDC_MSIME, hInst, NULL
		);
		controls_all++;
		wintools |= 0x10;
	}
	
	// Windows RE
	if (FileExist(TEXT("X:\\sources\\recovery\\recenv.exe")) == TRUE) {
		hWindowsREButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 205, 130, 20,
			hWnd, (HMENU)IDC_WINDOWSRE, hInst, NULL
		);
		controls_all++;
		wintools |= 0x20;
	}
	
	// Install Windows (NT6.x)
	if (FileExist(TEXT("X:\\sources\\setup.exe")) == TRUE) {
		hInstallWindowsButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			145, 205, 130, 20,
			hWnd, (HMENU)IDC_INSTALLWINDOWS, hInst, NULL
		);
		controls_all++;
		wintools |= 0x40;
	}
	
	// Other Applications
	
	// App1
	if (FileExist(uaFilePath1) == TRUE) {
		hApp1Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 230, 20, 20,
			hWnd, (HMENU)IDC_APP1, hInst, NULL
		);
		controls_all++;
		users_app |= 0x1;
	}
	
	// App2
	if (FileExist(uaFilePath2) == TRUE) {
		hApp2Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			35, 230, 20, 20,
			hWnd, (HMENU)IDC_APP2, hInst, NULL
		);
		controls_all++;
		users_app |= 0x2;
	}
	
	// App3
	if (FileExist(uaFilePath3) == TRUE) {
		hApp3Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			60, 230, 20, 20,
			hWnd, (HMENU)IDC_APP3, hInst, NULL
		);
		controls_all++;
		users_app |= 0x4;
	}
	
	// App4
	if (FileExist(uaFilePath4) == TRUE) {
		hApp4Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			85, 230, 20, 20,
			hWnd, (HMENU)IDC_APP4, hInst, NULL
		);
		controls_all++;
		users_app |= 0x8;
	}
	
	// App5
	if (FileExist(uaFilePath5) == TRUE) {
		hApp5Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			110, 230, 20, 20,
			hWnd, (HMENU)IDC_APP5, hInst, NULL
		);
		controls_all++;
		users_app |= 0x10;
	}
	
	// App6
	if (FileExist(uaFilePath6) == TRUE) {
		hApp6Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			135, 230, 20, 20,
			hWnd, (HMENU)IDC_APP6, hInst, NULL
		);
		controls_all++;
		users_app |= 0x20;
	}
	
	// App7
	if (FileExist(uaFilePath7) == TRUE) {
		hApp7Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			160, 230, 20, 20,
			hWnd, (HMENU)IDC_APP7, hInst, NULL
		);
		controls_all++;
		users_app |= 0x40;
	}
	
	// App8
	if (FileExist(uaFilePath8) == TRUE) {
		hApp8Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			185, 230, 20, 20,
			hWnd, (HMENU)IDC_APP8, hInst, NULL
		);
		controls_all++;
		users_app |= 0x80;
	}
	
	// App9
	if (FileExist(uaFilePath9) == TRUE) {
		hApp9Button = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			210, 230, 20, 20,
			hWnd, (HMENU)IDC_APP9, hInst, NULL
		);
		controls_all++;
		users_app |= 0x100;
	}
	
	// PStart or Custom USB Memory Launcher
	if (!lstrcmp(uNxPath, TEXT("true"))) {
		hPStartButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			235, 230, 20, 20,
			hWnd, (HMENU)IDC_PSTART, hInst, NULL
		);
		controls_all++;
	}
	
	// ��͓x�ύX
	if (FileExist(TEXT("setres.exe")) == TRUE) {
		// Default
		hResDefaultButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT,  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 255, 61, 16,
			hWnd, (HMENU)IDC_RESDEF, hInst, NULL
		);
		
		// XGA
		hResXGAButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			73, 255, 61, 16,
			hWnd, (HMENU)IDC_RESXGA, hInst, NULL
		);
		
		// SXGA
		hResSXGAButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 272, 61, 16,
			hWnd, (HMENU)IDC_RESSXGA, hInst, NULL
		);
		
		// HD
		hResHDButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			73, 272, 61, 16,
			hWnd, (HMENU)IDC_RESHD, hInst, NULL
		);
		
		// Res1 (edit)
		hRes1 = CreateWindowEx(WS_EX_CLIENTEDGE,
			TEXT("EDIT"), TEXT("0"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
			150, 262, 38, 20,
			hWnd, (HMENU)IDC_RES1, hInst, NULL
		);
		SendMessage(hRes1, WM_SETFONT, (WPARAM)Controls::hGUIFont, TRUE); // �t�H���g�ݒ�
		SendMessage(hRes1, EM_SETLIMITTEXT, 5, 0); // ���͏���ݒ�
		
		// Res2 (edit)
		hRes2 = CreateWindowEx(WS_EX_CLIENTEDGE,
			TEXT("EDIT"), TEXT("0"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
			196, 262, 38, 20,
			hWnd, (HMENU)IDC_RES2, hInst, NULL
		);
		SendMessage(hRes2, WM_SETFONT, (WPARAM)Controls::hGUIFont, TRUE);
		SendMessage(hRes2, EM_SETLIMITTEXT, 5, 0);
		
		// ResChange
		hResChangeButton = CreateWindowEx(0,
			TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			237, 263, 45, 16,
			hWnd, (HMENU)IDC_RESCHANGE, hInst, NULL
		);
		controls_all += 7;
		
		bSetRes = TRUE;
	} else {
		bSetRes = FALSE;
	}
	
	// Reboot WinPE
	hRebootButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 325, 130, 20,
		hWnd, (HMENU)IDC_REBOOTWINPE, hInst, NULL
	);
	controls_all++;
	
	// Shutdown WinPE
	hShutdownButton = CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		150, 325, 130, 20,
		hWnd, (HMENU)IDC_SHUTDOWNWINPE, hInst, NULL
	);
	controls_all++;
	
	// �쐬���ꂽ�{�^���̐������z����m��
	controlinfo = (CONTROLINFO *)malloc(sizeof(CONTROLINFO) * controls_all);
	if (controlinfo == NULL)
		return 1;

	// ���ۂɒl��ݒ肵���{�^���̐�
	int controls_set = 0;
	
	// �C���[�W�����[�h
	LoadButtonImages(hWnd, 3);
	
	// �t�@�C���[
	if (filer) {
		// �摜�����[�h
		LoadButtonImages(hWnd, 1);
		
		switch (filer)
		{
			case 1:
				SetImageButtonInfo(controls_set, hFilerButton, HDC_MEMID1, 0, 40, 180, 40, 180, 40);
				break;
			
			case 2:
				SetImageButtonInfo(controls_set, hQDirButton, HDC_MEMID1, 0, 20, 180, 20, 180, 20);
				break;
			
			case 3:
				SetImageButtonInfo(controls_set, h7ZipButton, HDC_MEMID1, 0, 0, 180, 0, 180, 0);
				break;
		}
		controls_set++;
	}
	
	// �t�@�C���I�[�v���_�C�A���O
	SetImageButtonInfo(controls_set, hFileOpenButton, HDC_MEMID4, 0, 60, 0, 80, 0, 80);
	controls_set++;
	
	if (backuptool || partitiontool)
		LoadButtonImages(hWnd, 2);
	
	// �o�b�N�A�b�v�c�[��
	if (backuptool) {
		switch (backuptool)
		{
			case 1:
				SetImageButtonInfo(controls_set, hAcroBackRecoverButton, HDC_MEMID2, 0, 0, 210, 0, 210, 0);
				break;
			
			case 2:
				SetImageButtonInfo(controls_set, hAcroTrueImage2009Button, HDC_MEMID2, 0, 0, 210, 0, 210, 0);
				break;
			
			case 3:
				SetImageButtonInfo(controls_set, hAcroTrueImage11Button, HDC_MEMID2, 0, 0, 210, 0, 210, 0);
				break;
			
			case 4:
				SetImageButtonInfo(controls_set, hAcroTrueImage10or9HomeButton, HDC_MEMID2, 0, 0, 210, 0, 210, 0);
				break;
			
			case 5:
				SetImageButtonInfo(controls_set, hAcroTrueImageLEorPersonal2Button, HDC_MEMID2, 0, 0, 210, 0, 210, 0);
				break;
			
			case 6:
				SetImageButtonInfo(controls_set, hDiscWizardButton, HDC_MEMID2, 0, 20, 210, 20, 210, 20);
				break;
			
			case 7:
				SetImageButtonInfo(controls_set, hParagonBackupAndRecoveryButton, HDC_MEMID2, 0, 40, 210, 40, 210, 40);
				break;
			
			case 8:
				SetImageButtonInfo(controls_set, hEaseUSTodoBackup25PEButton, HDC_MEMID2, 0, 60, 210, 60, 210, 60);
				break;
			
			case 9:
				SetImageButtonInfo(controls_set, hEaseUSTodoBackup4002PEButton, HDC_MEMID2, 0, 60, 210, 60, 210, 60);
				break;
			
			case 10:
				SetImageButtonInfo(controls_set, hEaseUSTodoBackup25WinButton, HDC_MEMID2, 0, 60, 210, 60, 210, 60);
				break;
			
			case 11:
				SetImageButtonInfo(controls_set, hOtherBackupToolButton, HDC_MEMID2, 0, 160, 210, 160, 210, 160);
				break;
		}
		controls_set++;
	}
	
	// �p�[�e�B�V�����c�[��
	if (partitiontool) {
		switch (partitiontool)
		{
			case 1:
				SetImageButtonInfo(controls_set, hEaseUSPartitionMasterButton, HDC_MEMID2, 0, 80, 210, 80, 210, 80);
				break;
			
			case 2:
				SetImageButtonInfo(controls_set, hPartitionWizardButton, HDC_MEMID2, 0, 100, 210, 100, 210, 100);
				break;
			
			case 3:
				SetImageButtonInfo(controls_set, hAcronisDiskDirectorButton, HDC_MEMID2, 0, 120, 210, 120, 210, 120);
				break;
			
			case 4:
				SetImageButtonInfo(controls_set, hParagonPartitionManagerButton, HDC_MEMID2, 0, 140, 210, 140, 210, 140);
				break;
			
			case 5:
				SetImageButtonInfo(controls_set, hOtherPartitionToolButton, HDC_MEMID2, 0, 160, 210, 160, 210, 160);
				break;
		}
		controls_set++;
	}
	
	// �W���I��Windows�A�v���P�[�V����
	SetImageButtonInfo(controls_set, hCommandPromptButton, HDC_MEMID3, 0, 40, 130, 40, 130, 40);
	SetImageButtonInfo(controls_set+1, hNotepadButton, HDC_MEMID3, 0, 60, 130, 60, 130, 60);
	SetImageButtonInfo(controls_set+2, hRegistryEditorButton, HDC_MEMID3, 0, 80, 130, 80, 130, 80);
	SetImageButtonInfo(controls_set+3, hDiskpartButton, HDC_MEMID3, 0, 100, 130, 100, 130, 100);
	SetImageButtonInfo(controls_set+4, hLoadDriverButton, HDC_MEMID3, 0, 120, 130, 120, 130, 120);
	controls_set += 5;
	
	// �I�v�V�����H
	if (wintools & 0x10) {
		SetImageButtonInfo(controls_set, hMSIMEButton, HDC_MEMID3, 0, 140, 130, 140, 130, 140);
		controls_set++;
	}
	if (wintools & 0x20) {
		SetImageButtonInfo(controls_set, hWindowsREButton, HDC_MEMID3, 0, 160, 130, 160, 130, 160);
		controls_set++;
	}
	if (wintools & 0x40) {
		SetImageButtonInfo(controls_set, hInstallWindowsButton, HDC_MEMID3, 0, 180, 130, 180, 130, 180);
		controls_set++;
	}
	
	// User's Applications (App1~9)
	LoadButtonImages(hWnd, 4);
	
	if (users_app & 0x1) {
		SetImageButtonInfo(controls_set, hApp1Button, HDC_MEMID4, 0, 100, 0, 100, 0, 100);
		controls_set++;
	}
	if (users_app & 0x2) {
		SetImageButtonInfo(controls_set, hApp2Button, HDC_MEMID4, 0, 120, 0, 120, 0, 120);
		controls_set++;
	}
	if (users_app & 0x4) {
		SetImageButtonInfo(controls_set, hApp3Button, HDC_MEMID4, 0, 140, 0, 140, 0, 140);
		controls_set++;
	}
	if (users_app & 0x8) {
		SetImageButtonInfo(controls_set, hApp4Button, HDC_MEMID4, 0, 160, 0, 160, 0, 160);
		controls_set++;
	}
	if (users_app & 0x10) {
		SetImageButtonInfo(controls_set, hApp5Button, HDC_MEMID4, 0, 180, 0, 180, 0, 180);
		controls_set++;
	}
	if (users_app & 0x20) {
		SetImageButtonInfo(controls_set, hApp6Button, HDC_MEMID4, 0, 200, 0, 200, 0, 200);
		controls_set++;
	}
	if (users_app & 0x40) {
		SetImageButtonInfo(controls_set, hApp7Button, HDC_MEMID4, 0, 220, 0, 220, 0, 220);
		controls_set++;
	}
	if (users_app & 0x80) {
		SetImageButtonInfo(controls_set, hApp8Button, HDC_MEMID4, 0, 240, 0, 240, 0, 240);
		controls_set++;
	}
	if (users_app & 0x100) {
		SetImageButtonInfo(controls_set, hApp9Button, HDC_MEMID4, 0, 260, 0, 260, 0, 260);
		controls_set++;
	}
	
	// PStart
	if (!lstrcmp(uNxPath, TEXT("true"))) {
		SetImageButtonInfo(controls_set, hPStartButton, HDC_MEMID4, 0, 40, 0, 40, 0, 40);
		controls_set++;
	}
	
	// SetRes
	if (bSetRes == TRUE) {
		// �摜�����[�h
		LoadButtonImages(hWnd, 5);
		
		SetImageButtonInfo(controls_set, hResDefaultButton, HDC_MEMID5, 0, 0, 61, 0, 61, 0);
		SetImageButtonInfo(controls_set+1, hResXGAButton, HDC_MEMID5, 0, 16, 61, 16, 61, 16);
		SetImageButtonInfo(controls_set+2, hResSXGAButton, HDC_MEMID5, 0, 32, 61, 32, 61, 32);
		SetImageButtonInfo(controls_set+3, hResHDButton, HDC_MEMID5, 0, 48, 61, 48, 61, 48);
		/* �ȉ��͓��̓{�b�N�X�����ATab�L�[�̃t�H�[�J�X�ړ����������邽�߁A�{�^���̃n���h���̂ݓ���� */
		SetImageButtonInfo(controls_set+4, hRes1, -1, 0, 0, 0, 0, 0, 0);
		SetImageButtonInfo(controls_set+5, hRes2, -1, 0, 0, 0, 0, 0, 0);
		controlinfo[controls_set+4].option = 0x10;
		controlinfo[controls_set+5].option = 0x10;
		
		SetImageButtonInfo(controls_set+6, hResChangeButton, HDC_MEMID5, 16, 64, 61, 64, 61, 64);
		controls_set += 7;
	}
	
	// Reboot / Shutdown
	SetImageButtonInfo(controls_set, hRebootButton, HDC_MEMID3, 0, 0, 130, 0, 130, 0);
	SetImageButtonInfo(controls_set+1, hShutdownButton, HDC_MEMID3, 0, 20, 130, 20, 130, 20);
	controls_set += 2;

	// �{�^���̃v���V�[�W���[���擾 (�ǂ̃{�^���̂��̂������Ȃ̂ň��̂�)
	DefButtonProc = (WNDPROC)GetWindowLongPtr(controlinfo[0].hWnd_control, GWLP_WNDPROC);
	
	for(int i = 0; i < controls_all; i++)
	{
		// index��ݒ�
		SetWindowLongPtr(controlinfo[i].hWnd_control, GWLP_USERDATA, i);
		
		if (controlinfo[i].hdc_id == -1)
			continue;

		// �{�^���̃T�u�N���X��
		SetWindowLongPtr(controlinfo[i].hWnd_control, GWLP_WNDPROC, (LONG_PTR)ImgButtonProc);

		// �C���[�W�{�^���� (BS_OWNERDRAW�X�^�C����K�p)
		SendMessage(controlinfo[i].hWnd_control, BM_SETSTYLE, BS_OWNERDRAW, TRUE);
	}
	
	return 0;
}

// �C���[�W�{�^���̏����Z�b�g
void Controls::SetImageButtonInfo(int id, HWND hWnd_button, int hdc_imgid, int pos_x1, int pos_y1, int pos_x2, int pos_y2, int pos_x3, int pos_y3)
{
	CONTROLINFO *lpControlInfo;

	lpControlInfo = &controlinfo[id];

	lpControlInfo->hWnd_control = hWnd_button;
	lpControlInfo->hdc_id = hdc_imgid;
	lpControlInfo->normal_x = pos_x1;
	lpControlInfo->normal_y = pos_y1;
	lpControlInfo->focus_x = pos_x2;
	lpControlInfo->focus_y = pos_y2;
	lpControlInfo->push_x = pos_x3;
	lpControlInfo->push_y = pos_y3;
	lpControlInfo->drawmode = 0;
	lpControlInfo->option = 0;
}

// �{�^���p�̉摜�ǂݍ���
void Controls::LoadButtonImages(HWND hWnd, int id)
{
	HDC hdc;
	HBITMAP hBitmap; // �g����
	HBRUSH hBrush_1;
	
	if (id > 5) return;
	
	// �_���u���V���쐬
	hBrush_1 = CreateSolidBrush(RGB(252, 254, 252));
	
	// ����Window�̃f�o�C�X�R���e�L�X�g�n���h�����擾
	hdc = GetDC(hWnd);
	
	switch (id)
	{
		case 1:
			// �摜1
			hdc_mem[0] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 360, 60);
			SelectObject(hdc_mem[0], hBitmap);
			Boxfill(hdc_mem[0], 0, 0, 360, 60, hBrush_1); // �h��Ԃ�
			if (LoadPictureOnResource(hdc_mem[0], MAKEINTRESOURCE(IDG_IMG7ZIP), TEXT("GIF")) == FALSE)
#ifdef PECMD_JAPANESE
				MessageBox(NULL, TEXT("�C���[�W�̓ǂݍ��݂Ɏ��s���܂����B(1)"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(NULL, TEXT("Failed to load the image. (1)"), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
			break;
		
		case 2:
			// �摜2
			hdc_mem[1] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 420, 180);
			SelectObject(hdc_mem[1], hBitmap);
			Boxfill(hdc_mem[1], 0, 0, 420, 180, hBrush_1); // �h��Ԃ�
			if (LoadPictureOnResource(hdc_mem[1], MAKEINTRESOURCE(IDG_IMGDISK), TEXT("GIF")) == FALSE)
#ifdef PECMD_JAPANESE
				MessageBox(NULL, TEXT("�C���[�W�̓ǂݍ��݂Ɏ��s���܂����B(2)"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(NULL, TEXT("Failed to load the image. (2)"), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
			break;
		
		case 3:
			// �摜3
			hdc_mem[2] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 260, 200);
			SelectObject(hdc_mem[2], hBitmap);
			Boxfill(hdc_mem[2], 0, 0, 260, 200, hBrush_1); // �h��Ԃ�
			if (LoadPictureOnResource(hdc_mem[2], MAKEINTRESOURCE(IDG_IMGTOOLS), TEXT("GIF")) == FALSE)
#ifdef PECMD_JAPANESE
				MessageBox(NULL, TEXT("�C���[�W�̓ǂݍ��݂Ɏ��s���܂����B(3)"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(NULL, TEXT("Failed to load the image. (3)"), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
			break;
		
		case 4:
			{
				// �摜4 (apps)
				TCHAR szLaunchPath[MAX_PATH + 1];
				HBRUSH hBrush_2;
				hBrush_2 = CreateSolidBrush(RGB(236, 234, 164));
				
				hdc_mem[3] = CreateCompatibleDC(NULL);
				hBitmap = CreateCompatibleBitmap(hdc, 20, 300);
				SelectObject(hdc_mem[3], hBitmap);
				Boxfill(hdc_mem[3], 0, 0, 20, 300, hBrush_1); // �h��Ԃ�
				if (LoadPictureOnResource(hdc_mem[3], MAKEINTRESOURCE(IDG_IMGAPPS), TEXT("GIF")) == FALSE)
#ifdef PECMD_JAPANESE
					MessageBox(NULL, TEXT("�C���[�W�̓ǂݍ��݂Ɏ��s���܂����B(4)"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
					MessageBox(NULL, TEXT("Failed to load the image. (4)"), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
				SelectObject(hdc_mem[3], hBrush_1);
				
				// �g��`��
				RoundRect(hdc_mem[3], 0, 60, 20, 80, 6, 6);
				SelectObject(hdc_mem[3], hBrush_2); // �F��ύX
				RoundRect(hdc_mem[3], 0, 80, 20, 100, 6, 6);
				SelectObject(hdc_mem[3], hBrush_1); // ���ɖ߂�
				RoundRect(hdc_mem[3], 0, 100, 20, 120, 6, 6);
				RoundRect(hdc_mem[3], 0, 120, 20, 140, 6, 6);
				RoundRect(hdc_mem[3], 0, 140, 20, 160, 6, 6);
				RoundRect(hdc_mem[3], 0, 160, 20, 180, 6, 6);
				RoundRect(hdc_mem[3], 0, 180, 20, 200, 6, 6);
				RoundRect(hdc_mem[3], 0, 200, 20, 220, 6, 6);
				RoundRect(hdc_mem[3], 0, 220, 20, 240, 6, 6);
				RoundRect(hdc_mem[3], 0, 240, 20, 260, 6, 6);
				RoundRect(hdc_mem[3], 0, 260, 20, 280, 6, 6);
				RoundRect(hdc_mem[3], 0, 280, 20, 300, 6, 6);

				// �t�H���_�A�C�R����`��
				DrawFolderImage(hdc_mem[3], 62, 82);
				
				// �t�@�C���A�C�R����`��
				DrawFileImage(hdc_mem[3], uaFilePath1, 2, 102);
				DrawFileImage(hdc_mem[3], uaFilePath2, 2, 122);
				DrawFileImage(hdc_mem[3], uaFilePath3, 2, 142);
				DrawFileImage(hdc_mem[3], uaFilePath4, 2, 162);
				DrawFileImage(hdc_mem[3], uaFilePath5, 2, 182);
				DrawFileImage(hdc_mem[3], uaFilePath6, 2, 202);
				DrawFileImage(hdc_mem[3], uaFilePath7, 2, 222);
				DrawFileImage(hdc_mem[3], uaFilePath8, 2, 242);
				DrawFileImage(hdc_mem[3], uaFilePath9, 2, 262);
				
				// USB���������̃����`���[�̃A�C�R�����擾 (�擾�ł��Ȃ�������PStart�̂܂�)
				for(int i = 0; i < 26; i++)
				{
					wsprintf(szLaunchPath, TEXT("%c:\\%s"), 'A'+i, ulPath);
					if (FileExist(szLaunchPath) == TRUE) {
						// �h��Ԃ��Ă���`��
						Boxfill(hdc_mem[3], 2, 42, 17, 57, hBrush_1);
						DrawFileImage(hdc_mem[3], szLaunchPath, 2, 42);
						break;
					}
				}
			
				// ���낢��폜
				DeleteObject(hBrush_2);
			}
			break;
		
		case 5:
			// �摜5
			hdc_mem[4] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 122, 80);
			SelectObject(hdc_mem[4], hBitmap);
			Boxfill(hdc_mem[4], 0, 0, 122, 80, hBrush_1); // �h��Ԃ�
			if (LoadPictureOnResource(hdc_mem[4], MAKEINTRESOURCE(IDG_IMGRES), TEXT("GIF")) == FALSE)
#ifdef PECMD_JAPANESE
				MessageBox(NULL, TEXT("�C���[�W�̓ǂݍ��݂Ɏ��s���܂����B(5)"), TEXT("�G���["), MB_OK | MB_ICONERROR);
#else
				MessageBox(NULL, TEXT("Failed to load the image. (5)"), TEXT("Error"), MB_OK | MB_ICONERROR);
#endif
			break;
		
	}
	
	DeleteObject(hBitmap);
	DeleteObject(hBrush_1);
	ReleaseDC(hWnd, hdc);
}

// �R���g���[���̃e�L�X�g���擾
int Controls::GetResText(TCHAR *lpResBuf, int flag)
{
	switch (flag)
	{
		case 1:
			SendMessage(hRes1, WM_GETTEXT, 6/*5(������) +1(NULL������)*/, (LPARAM)lpResBuf);
			break;
		
		case 2:
			SendMessage(hRes2, WM_GETTEXT, 6, (LPARAM)lpResBuf);
			break;
		
		default:
			return -1;
		
	}
	return 0;
}

// �}�E�X�C�x���g���̃{�^���`�揈���̑O����
void Controls::DrawImageButtonOnMouseEvent(HWND hWnd_button, int flag)
{
	HDC hdc_button;
	int id;
	CONTROLINFO *lpControlInfo;
	id = (int)GetWindowLongPtr(hWnd_button, GWLP_USERDATA);
	lpControlInfo = &controlinfo[id];
	
	if (lpControlInfo->hWnd_control != hWnd_button)
		return;
	
	if (flag) {
		if (lpControlInfo->option & 0x1)
			return;
		
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd_button;
		TrackMouseEvent(&tme);
		
		lpControlInfo->option |= 0x1;
	} else {
		if ((lpControlInfo->option & 0x1) == 0)
			return;
		lpControlInfo->option &= ~0x1;
	}
	lpControlInfo->drawmode = flag;
	
	hdc_button = GetDC(hWnd_button);
	DrawImageButton(lpControlInfo, hdc_button);
	ReleaseDC(hWnd_button, hdc_button);
	
	return;
}

// �I�[�i�[�h���[���̕`�揈���̑O����
void Controls::DrawImageButtonOnOwnerDraw(HWND hWnd_button, HDC hdc_button, UINT state)
{
	int id;
	CONTROLINFO *lpControlInfo;
	id = (int)GetWindowLongPtr(hWnd_button, GWLP_USERDATA);
	lpControlInfo = &controlinfo[id];
	
	if (lpControlInfo->hWnd_control != hWnd_button)
		return;
	
	if (state & ODS_FOCUS ) {
		lpControlInfo->option |= 0x2;
	} else {
		lpControlInfo->option &= ~0x2;
	}
	
	if (state & ODS_SELECTED )
		lpControlInfo->drawmode = 2;
	
	DrawImageButton(lpControlInfo, hdc_button);
	
	return;
}

void Controls::DrawImageButton(CONTROLINFO *lpControlInfo, HDC hdc_button)
{
	int img_x, img_y;
	RECT rect;
	HWND hWnd_Button = lpControlInfo->hWnd_control;
	
	// �{�^���̑傫�����擾
	GetClientRect(hWnd_Button, &rect);
	
	// �`�惂�[�h�ɉ����Ĉʒu��ݒ�
	switch (lpControlInfo->drawmode)
	{
		case 1:
			img_x = lpControlInfo->focus_x;
			img_y = lpControlInfo->focus_y;
			break;
		
		case 2:
			img_x = lpControlInfo->push_x;
			img_y = lpControlInfo->push_y;
			break;
		
		default:
			img_x = lpControlInfo->normal_x;
			img_y = lpControlInfo->normal_y;
			break;
	}
	
	// �{�^���ɉ摜��]��
	BitBlt(hdc_button, rect.left, rect.top, rect.right, rect.bottom, hdc_mem[lpControlInfo->hdc_id], img_x, img_y, SRCCOPY);
	
	if (lpControlInfo->option & 0x2) {
		// �{�^���I���� (�L�[�{�[�h�t�H�[�J�X�L��)
		InflateRect(&rect, -3, -3);
		DrawFocusRect(hdc_button, &rect);
	}
	
	return;
}

// TAb�L�[�̃t�H�[�J�X�ړ�������
void Controls::SetTabKeyFocus(int key_flag)
{
	HWND hWnd_focus, hWnd;
	int i, activectlid = -1;
	CONTROLINFO *lpControl;
	
	hWnd_focus = GetFocus();
	if (hWnd_focus != NULL) {
		for(i = 0; i < controls_all; i++) {
			if (hWnd_focus == controlinfo[i].hWnd_control) activectlid = i;
		}
	}

	for(i = 0; i < controls_all; i++) {
		activectlid += key_flag;
		if (activectlid >= controls_all)
			activectlid = 0;
		if (activectlid < 0)
			activectlid = controls_all - 1;
		lpControl = &controlinfo[activectlid];
		hWnd = lpControl->hWnd_control;
		if (hWnd != NULL) {
			if (IsWindowEnabled(hWnd)) {
				if (lpControl->option & 0x10) {
					SendMessage(hWnd, EM_SETSEL, 0, -1);
				}
				SetFocus(hWnd);
				return;
			}
		}
	}
}
