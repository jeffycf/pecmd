// controls.cpp : Launcher PEのボタンなどのウィンドウオブジェクトを作成
//              : イメージボタンの管理も行う

#include <windows.h>
#include <tchar.h>

#include "resource.h"
#include "controls.h"
#include "FileFunc.h"
#include "graphic.h"
#include "pecmd.h"
#include "lang.h"

// HDCの何か
#define HDC_MEMID1 0
#define HDC_MEMID2 1
#define HDC_MEMID3 2
#define HDC_MEMID4 3
#define HDC_MEMID5 4

static WNDPROC DefButtonProc;
static WNDPROC DefEditProc;

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

LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if ((msg == WM_CHAR) && (wParam == 9 || wParam == 13))
		return 0;
	
	return CallWindowProc(DefEditProc, hWnd, msg, wParam, lParam);
}

Controls::Controls(void)
{
	// GUIフォントを取得
	hGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}

Controls::~Controls(void)
{
	// GUIフォントを削除
	DeleteObject(hGUIFont);
	
	// イメージボタン情報を削除
	free(controlinfo);
	
	// デバイスコンテキストハンドルを破棄
	if (hdc_mem[0]) DeleteDC(hdc_mem[0]);
	if (hdc_mem[1]) DeleteDC(hdc_mem[1]);
	if (hdc_mem[2]) DeleteDC(hdc_mem[2]);
	if (hdc_mem[3]) DeleteDC(hdc_mem[3]);
	if (hdc_mem[4]) DeleteDC(hdc_mem[4]);
}

// ボタンの作成とイメージボタン化 (長すぎる)
int Controls::CreateControls(HWND hWnd, HINSTANCE hInst)
{
	int partitiontool = 0;
	int filer_pos = 0, backuptool_pos = 0, partitiontool_pos = 0;
	long ctl_id = 0;
	UINT wintools = 0L, users_app = 0L;
	
	// ボタンのハンドル
	HWND hFilerButton, hFileOpenButton, hBackupToolButton, hPartitionToolButton, hCommandPromptButton, hNotepadButton, hRegistryEditorButton, hDiskpartButton, hLoadDriverButton, hMSIMEButton, hWindowsREButton, hInstallWindowsButton, hApp1Button, hApp2Button, hApp3Button, hApp4Button, hApp5Button, hApp6Button, hApp7Button, hApp8Button, hApp9Button, hPStartButton, hResDefaultButton, hResXGAButton, hResSXGAButton, hResHDButton, hResChangeButton, hRebootButton, hShutdownButton;
	
	controls_all = 0;
	hFilerButton = hBackupToolButton = hPartitionToolButton = NULL;
	
	/*---------------------------------- ここから大量のボタンを作成 ----------------------------------*/
	
	// 画像ボタンにするときはボタンにフォント設定しないので(というかしなくてもいいので)、WM_SETFONTはボタンには適用しない
	
	// ファイラー
	if (FileExist(ufFilePath)) {
		// ユーザーが自由に指定できるファイラー
		ctl_id = IDC_FILER;
		filer_pos = 40;
	}
	else if (FileExist(TEXT("X:\\Program Files\\q-dir\\q-dir.exe"))) {
		// Q-Dirが存在する場合
		ctl_id = IDC_QDIR;
		filer_pos = 20;
	}
	else if (FileExist(TEXT("X:\\Program Files\\7-Zip\\7zFM.exe"))) {
		// Q-Dirが存在しない & 7-Zipが存在する場合
		ctl_id = IDC_7ZIP;
	}
	// ファイラーのボタン作成
	if (ctl_id) {
		hFilerButton = CreateButton(hWnd, hInst, 25, 20, 180, 20, ctl_id);
		controls_all++;
	}
	
	// ファイルオープンダイアログ, File Open Dialog
	hFileOpenButton = CreateButton(hWnd, hInst, 213, 20, 20, 20, IDC_FILEOPENDLG);
	
	ctl_id = 0;
	
	// バックアップツール, Backup Tools
	if (FileExist(TEXT("X:\\Program Files\\Acronis\\BackupAndRecovery\\trueimage_starter.exe"))) {
		// Acronis Backup & Recovery 10
		ctl_id = IDC_ACROBACKRECOVER;
	}
	else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImageHome\\trueimage_starter.exe"))) {
		// TrueImage2009以降
		ctl_id = IDC_ACROTRUEIMAGE2009;
	}
	else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImage_starter.exe"))) {
		// TrueImage11
		ctl_id = IDC_ACROTRUEIMAGE11;
	}
	else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImage.exe"))) {
		// TrueImage10/9Home
		ctl_id = IDC_ACROTRUEIMAGE10OR9HOME;
	}
	else if (FileExist(TEXT("X:\\Program Files\\Acronis\\TrueImage\\TrueImage.exe"))) {
		// TrueImageLE/Personal2
		Execute(NULL, NULL, TEXT("reg.exe"), TEXT("add HKCU\\Software\\Acronis\\TrueImage\\DontShow /v Updates /t REG_SZ /d 1 /f"), NULL, SW_SHOWMINIMIZED); // 最小化状態で実行
		Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage\\schedhlp.exe"), NULL, NULL, SW_SHOWDEFAULT);
		Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage\\TimounterMonitor.exe"), NULL, NULL, SW_SHOWDEFAULT);
		Execute(NULL, NULL, TEXT("X:\\Program Files\\Acronis\\TrueImage\\TrueImageMonitor.exe"), NULL, NULL, SW_SHOWDEFAULT);
		ctl_id = IDC_ACROTRUEIMAGELEORPERSONAL2;
	}
	else if (FileExist(TEXT("X:\\Program Files\\Seagate\\DiscWizard_starter.exe"))) {
		// DiscWizard
		ctl_id = IDC_DISCWIZARD;
		backuptool_pos = 20;
	}
	else if (FileExist(TEXT("X:\\Program Files\\PBR\\program\\launcher.lnk"))) {
		// Paragon Backup & Recovery
		ctl_id = IDC_PARAGBACKRECOVER;
		backuptool_pos = 40;
	}
	else if (FileExist(TEXT("X:\\Program Files\\easeus\\tb2.0\\bin\\PELoader.lnk"))) {
		// EASEUS Todo Backup 2.5x (公式PE互換)
		ctl_id = IDC_EASEUSTODOBACKUP25PE;
		backuptool_pos = 60;
	}
	else if (FileExist(TEXT("X:\\Program Files\\easeus\\tb\\bin\\PELoader.lnk"))) {
		// EaseUS Todo Backup 4.0.0.2 (公式PE互換)
		ctl_id = IDC_EASEUSTODOBACKUP4002PE;
		backuptool_pos = 60;
	}
	else if (FileExist(TEXT("X:\\Program Files\\ETB\\bin\\loader.lnk"))) {
		// EASEUS Todo Backup 2.5x (Win版互換)
		ctl_id = IDC_EASEUSTODOBACKUP25WIN;
		backuptool_pos = 60;
	}
	else if (FileExist(TEXT("X:\\Program Files\\otherbpt.lnk"))) {
		// 他のバックアップツール
		ctl_id = IDC_OTHERBACKUPTOOL;
		backuptool_pos = 160;
	}
	// バックアップツールのボタンを作成
	if (ctl_id)
		hBackupToolButton = CreateButton(hWnd, hInst, 25, 50, 210, 20, ctl_id);
	
	ctl_id = 0;
	
	// パーティションツール
	if (FileExist(TEXT("X:\\Program Files\\EPM\\bin\\epm0.lnk"))) {
		// EaseUS Partition Master
		ctl_id = IDC_EASEUSPARTITIONMASTER;
		partitiontool_pos = 80;
	}
	else if (FileExist(TEXT("X:\\Program Files\\MPW\\PartitionWizard.lnk"))) {
		// Partition Wizard
		ctl_id = IDC_PARTITIONWIZARD;
		partitiontool_pos = 100;
	}
	else if (FileExist(TEXT("X:\\Program Files\\Acronis\\DiskDirector\\trueimage_starter.exe"))) {
		// Acronis Disk Director
		ctl_id = IDC_ACRODISKDIRECTOR11;
		partitiontool_pos = 120;
	}
	else if (FileExist(TEXT("X:\\Program Files\\PPM\\program\\launcher.lnk"))) {
		// Paragon Partition Manager
		ctl_id = IDC_PARAGPARTITIONMANAGER;
		partitiontool_pos = 140;
	}
	else if (FileExist(TEXT("X:\\Program Files\\otherptt.lnk"))) {
		// 他のパーティションツール
		ctl_id = IDC_OTHERPARTITIONTOOL;
		partitiontool_pos = 160;
	}
	// パーティションツールのボタンを作成
	if (ctl_id)
		hPartitionToolButton = CreateButton(hWnd, hInst, 25, 80, 210, 20, ctl_id);
	
	// Windows Tools
	
	// Command Prompt (コマンドプロンプト)
	hCommandPromptButton = CreateButton(hWnd, hInst, 10, 130, 130, 20, IDC_COMMANDPROMPT);
	
	// Notepad (メモ帳)
	hNotepadButton = CreateButton(hWnd, hInst, 145, 130, 130, 20, IDC_NOTEPAD);
	
	// Registry Editor (レジストリエディター)
	hRegistryEditorButton = CreateButton(hWnd, hInst, 10, 155, 130, 20, IDC_REGISTRYEDITOR);
	
	// Diskpart
	hDiskpartButton = CreateButton(hWnd, hInst, 145, 155, 130, 20, IDC_DISKPART);
	
	// Load Driver (ドライバ読み込み)
	hLoadDriverButton = CreateButton(hWnd, hInst, 10, 180, 130, 20, IDC_LOADDRIVER);
	
	// MS-IME
	if (FileExist(TEXT("imecmd.cmd"))) {
		hMSIMEButton = CreateButton(hWnd, hInst, 145, 180, 130, 20, IDC_MSIME);
		wintools |= 0x10;
	}
	
	// Windows RE
	if (FileExist(TEXT("X:\\sources\\recovery\\recenv.exe"))) {
		hWindowsREButton = CreateButton(hWnd, hInst, 10, 205, 130, 20, IDC_WINDOWSRE);
		wintools |= 0x20;
	}
	
	// Install Windows (NT6.x)
	if (FileExist(TEXT("X:\\sources\\setup.exe"))) {
		hInstallWindowsButton = CreateButton(hWnd, hInst, 145, 205, 130, 20, IDC_INSTALLWINDOWS);
		wintools |= 0x40;
	}
	
	// Other Applications
	
	// App1
	if (FileExist(uaFilePath1)) {
		hApp1Button = CreateButton(hWnd, hInst, 10, 230, 20, 20, IDC_APP1);
		users_app |= 0x1;
	}
	
	// App2
	if (FileExist(uaFilePath2)) {
		hApp2Button = CreateButton(hWnd, hInst, 35, 230, 20, 20, IDC_APP2);
		users_app |= 0x2;
	}
	
	// App3
	if (FileExist(uaFilePath3)) {
		hApp3Button = CreateButton(hWnd, hInst, 60, 230, 20, 20, IDC_APP3);
		users_app |= 0x4;
	}
	
	// App4
	if (FileExist(uaFilePath4)) {
		hApp4Button = CreateButton(hWnd, hInst, 85, 230, 20, 20, IDC_APP4);
		users_app |= 0x8;
	}
	
	// App5
	if (FileExist(uaFilePath5)) {
		hApp5Button = CreateButton(hWnd, hInst, 110, 230, 20, 20, IDC_APP5);
		users_app |= 0x10;
	}
	
	// App6
	if (FileExist(uaFilePath6)) {
		hApp6Button = CreateButton(hWnd, hInst, 135, 230, 20, 20, IDC_APP6);
		users_app |= 0x20;
	}
	
	// App7
	if (FileExist(uaFilePath7)) {
		hApp7Button = CreateButton(hWnd, hInst, 160, 230, 20, 20, IDC_APP7);
		users_app |= 0x40;
	}
	
	// App8
	if (FileExist(uaFilePath8)) {
		hApp8Button = CreateButton(hWnd, hInst, 185, 230, 20, 20, IDC_APP8);
		users_app |= 0x80;
	}
	
	// App9
	if (FileExist(uaFilePath9)) {
		hApp9Button = CreateButton(hWnd, hInst, 210, 230, 20, 20, IDC_APP9);
		users_app |= 0x100;
	}
	
	// PStart or Custom USB Memory Launcher
	if (!lstrcmp(uNxPath, TEXT("true"))) {
		hPStartButton = CreateButton(hWnd, hInst, 235, 230, 20, 20, IDC_PSTART);
	}
	
	// 解像度変更
	
	// Default
	hResDefaultButton = CreateButton(hWnd, hInst, 10, 255, 61, 16, IDC_RESDEF);
	
	// XGA
	hResXGAButton = CreateButton(hWnd, hInst, 73, 255, 61, 16, IDC_RESXGA);
	
	// SXGA
	hResSXGAButton = CreateButton(hWnd, hInst, 10, 272, 61, 16, IDC_RESSXGA);
	
	// HD
	hResHDButton = CreateButton(hWnd, hInst, 73, 272, 61, 16, IDC_RESHD);
	
	// Res1 (edit)
	hRes1 = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"), TEXT("0"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER,
		150, 262, 38, 20,
		hWnd, (HMENU)IDC_RES1, hInst, NULL
	);
	SendMessage(hRes1, WM_SETFONT, (WPARAM)hGUIFont, TRUE); // フォント設定
	SendMessage(hRes1, EM_SETLIMITTEXT, 5, 0); // 入力上限設定
	
	// Res2 (edit)
	hRes2 = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("EDIT"), TEXT("0"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER,
		196, 262, 38, 20,
		hWnd, (HMENU)IDC_RES2, hInst, NULL
	);
	SendMessage(hRes2, WM_SETFONT, (WPARAM)hGUIFont, TRUE);
	SendMessage(hRes2, EM_SETLIMITTEXT, 5, 0);
	
	// エディトボックスのプロシージャーを取得
	DefEditProc = (WNDPROC)GetWindowLongPtr(hRes1, GWLP_WNDPROC);

	// プロシージャーを設定
	SetWindowLongPtr(hRes1, GWLP_WNDPROC, (LONG_PTR)EditProc);
	SetWindowLongPtr(hRes2, GWLP_WNDPROC, (LONG_PTR)EditProc);

	// ResChange
	hResChangeButton = CreateButton(hWnd, hInst, 237, 263, 45, 16, IDC_RESCHANGE);
	controls_all += 2;
	
	// Reboot WinPE
	hRebootButton = CreateButton(hWnd, hInst, 10, 325, 130, 20, IDC_REBOOTWINPE);
	
	// Shutdown WinPE
	hShutdownButton = CreateButton(hWnd, hInst, 150, 325, 130, 20, IDC_SHUTDOWNWINPE);
	
	// 作成されたボタンの数だけ配列を確保
	controlinfo = (CONTROLINFO *)malloc(sizeof(CONTROLINFO) * controls_all);
	if (controlinfo == NULL)
		return 1;
	
	// 実際に値を設定したボタンの数
	int controls_set = 0;
	
	// イメージをロード
	LoadButtonImages(hWnd, 3);
	
	// ファイラー
	if (hFilerButton) {
		// 画像をロード
		LoadButtonImages(hWnd, 1);
		
		// 情報をセット
		SetImageButtonInfo(controls_set, hFilerButton, HDC_MEMID1, 0, filer_pos, 180, filer_pos, 180, filer_pos);
		controls_set++;
	}
	
	// ファイルオープンダイアログ
	SetImageButtonInfo(controls_set, hFileOpenButton, HDC_MEMID4, 0, 60, 0, 80, 0, 80);
	controls_set++;
	
	if (hBackupToolButton || hPartitionToolButton)
		LoadButtonImages(hWnd, 2);
	
	// バックアップツール
	if (hBackupToolButton) {
		SetImageButtonInfo(controls_set, hBackupToolButton, HDC_MEMID2, 0, backuptool_pos, 210, backuptool_pos, 210, backuptool_pos);
		controls_set++;
	}
	
	// パーティションツール
	if (hPartitionToolButton) {
		SetImageButtonInfo(controls_set, hPartitionToolButton, HDC_MEMID2, 0, partitiontool_pos, 210, partitiontool_pos, 210, partitiontool_pos);
		controls_set++;
	}
	
	// 標準的なWindowsアプリケーション
	SetImageButtonInfo(controls_set, hCommandPromptButton, HDC_MEMID3, 0, 40, 130, 40, 130, 40);
	SetImageButtonInfo(controls_set+1, hNotepadButton, HDC_MEMID3, 0, 60, 130, 60, 130, 60);
	SetImageButtonInfo(controls_set+2, hRegistryEditorButton, HDC_MEMID3, 0, 80, 130, 80, 130, 80);
	SetImageButtonInfo(controls_set+3, hDiskpartButton, HDC_MEMID3, 0, 100, 130, 100, 130, 100);
	SetImageButtonInfo(controls_set+4, hLoadDriverButton, HDC_MEMID3, 0, 120, 130, 120, 130, 120);
	controls_set += 5;
	
	// オプション？
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
	
	// 解像度変更
	LoadButtonImages(hWnd, 5);
	
	SetImageButtonInfo(controls_set, hResDefaultButton, HDC_MEMID5, 0, 0, 61, 0, 61, 0);
	SetImageButtonInfo(controls_set+1, hResXGAButton, HDC_MEMID5, 0, 16, 61, 16, 61, 16);
	SetImageButtonInfo(controls_set+2, hResSXGAButton, HDC_MEMID5, 0, 32, 61, 32, 61, 32);
	SetImageButtonInfo(controls_set+3, hResHDButton, HDC_MEMID5, 0, 48, 61, 48, 61, 48);
	/* 以下は入力ボックスだが、Tabキーのフォーカス移動を処理するため、ボタンのハンドルのみ入れる */
	SetImageButtonInfo(controls_set+4, hRes1, -1, 0, 0, 0, 0, 0, 0);
	SetImageButtonInfo(controls_set+5, hRes2, -1, 0, 0, 0, 0, 0, 0);
	controlinfo[controls_set+4].option = 0x10;
	controlinfo[controls_set+5].option = 0x10;
	
	SetImageButtonInfo(controls_set+6, hResChangeButton, HDC_MEMID5, 16, 64, 61, 64, 61, 64);
	controls_set += 7;
	
	// Reboot / Shutdown
	SetImageButtonInfo(controls_set, hRebootButton, HDC_MEMID3, 0, 0, 130, 0, 130, 0);
	SetImageButtonInfo(controls_set+1, hShutdownButton, HDC_MEMID3, 0, 20, 130, 20, 130, 20);
	controls_set += 2;
	
	// ボタンのプロシージャーを取得 (どのボタンのものも同じなので一回のみ)
	DefButtonProc = (WNDPROC)GetWindowLongPtr(controlinfo[0].hWnd_control, GWLP_WNDPROC);
	
	for(int i = 0; i < controls_all; i++)
	{
		// indexを設定
		SetWindowLongPtr(controlinfo[i].hWnd_control, GWLP_USERDATA, i);
		
		if (controlinfo[i].hdc_id == -1)
			continue;
		
		// ボタンのサブクラス化
		SetWindowLongPtr(controlinfo[i].hWnd_control, GWLP_WNDPROC, (LONG_PTR)ImgButtonProc);
		
		// イメージボタン化 (BS_OWNERDRAWスタイルを適用)
		SendMessage(controlinfo[i].hWnd_control, BM_SETSTYLE, BS_OWNERDRAW, TRUE);
	}
	
	return 0;
}

// ボタンを作成
HWND Controls::CreateButton(HWND hWnd, HINSTANCE hInst, int pos_x, int pos_y, int size_x, int size_y, long id)
{
	controls_all++;
	return CreateWindowEx(0,
		TEXT("BUTTON"), NULLTEXT, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		pos_x, pos_y, size_x, size_y,
		hWnd, (HMENU)id, hInst, NULL
	);
}

// イメージボタンの情報をセット
void Controls::SetImageButtonInfo(int id, HWND hWnd_button, int hdc_imgid, int pos_x1, int pos_y1, int pos_x2, int pos_y2, int pos_x3, int pos_y3)
{
	CONTROLINFO *lpControlInfo;
	
	lpControlInfo = &controlinfo[id];
	
	lpControlInfo->hWnd_control = hWnd_button;
	lpControlInfo->hdc_id = hdc_imgid;
	lpControlInfo->imgpos[0].x = pos_x1;
	lpControlInfo->imgpos[0].y = pos_y1;
	lpControlInfo->imgpos[1].x = pos_x2;
	lpControlInfo->imgpos[1].y = pos_y2;
	lpControlInfo->imgpos[2].x = pos_x3;
	lpControlInfo->imgpos[2].y = pos_y3;
	lpControlInfo->drawmode = 0;
	lpControlInfo->option = 0;
}

// ボタン用の画像読み込み
void Controls::LoadButtonImages(HWND hWnd, int id)
{
	HDC hdc;
	HBITMAP hBitmap; // 使い回し
	HBRUSH hBrush_1;
	
	if (id > 5) return;
	
	HFONT hGuiFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	
	// 論理ブラシを作成
	hBrush_1 = CreateSolidBrush(RGB(252, 254, 252));
	
	// 元のWindowのデバイスコンテキストハンドルを取得
	hdc = GetDC(hWnd);
	
	switch (id)
	{
		case 1:
			// 画像1
			hdc_mem[0] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 360, 60);
			SelectObject(hdc_mem[0], hBitmap);
			Boxfill(hdc_mem[0], 0, 0, 360, 60, hBrush_1); // 塗りつぶし
			if (!LoadPictureOnResource(hdc_mem[0], MAKEINTRESOURCE(IDG_IMG7ZIP)))
				ShowLoadPictureError(1);
			
			// 文字を描画
			SelectObject(hdc_mem[0], hGuiFont);
			SetBkMode(hdc_mem[0], TRANSPARENT);
			DrawStringForImageButton(hdc_mem[0], langStr.szBtn7Zip, 39, 5, 180, 17, 219, 5, 360, 17, FALSE);
			DrawStringForImageButton(hdc_mem[0], langStr.szBtnComputer, 39, 25, 106, 37, 219, 25, 286, 37, FALSE);
			DrawStringForImageButton(hdc_mem[0], TEXT("["), 107, 25, 114, 57, 287, 25, 294, 57, FALSE);
			DrawStringForImageButton(hdc_mem[0], langStr.szBtnQDir, 135, 25, 180, 77, 315, 25, 360, 77, FALSE);
			DrawStringForImageButton(hdc_mem[0], langStr.szBtnUsersFiler, 39, 45, 180, 97, 219, 45, 360, 97, FALSE);
			
			break;
		
		case 2:
			// 画像2
			hdc_mem[1] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 420, 180);
			SelectObject(hdc_mem[1], hBitmap);
			Boxfill(hdc_mem[1], 0, 0, 420, 180, hBrush_1); // 塗りつぶし
			if (!LoadPictureOnResource(hdc_mem[1], MAKEINTRESOURCE(IDG_IMGDISK)))
				ShowLoadPictureError(2);
			
			// 文字を描画
			SelectObject(hdc_mem[1], hGuiFont);
			SetBkMode(hdc_mem[1], TRANSPARENT);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnTrueImage, 27, 5, 210, 17, 237, 5, 420, 17, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnDiscWizard, 27, 25, 210, 37, 237, 25, 420, 37, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnParagonBackupAndRecovery, 27, 45, 210, 57, 237, 45, 420, 57, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnEaseUSTodoBackup, 27, 65, 210, 77, 237, 65, 420, 77, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnEaseUSPartitionMaster, 27, 85, 210, 97, 237, 85, 420, 97, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnMiniToolPartitionWizard, 27, 105, 210, 117, 237, 105, 420, 117, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnAcronisDiskDirector, 27, 125, 210, 137, 237, 125, 420, 137, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnParagonPartitionManager, 27, 145, 210, 157, 237, 145, 420, 157, FALSE);
			DrawStringForImageButton(hdc_mem[1], langStr.szBtnOtherDiskTool, 27, 165, 210, 177, 237, 165, 420, 177, FALSE);
			
			break;
		
		case 3:
			// 画像3
			hdc_mem[2] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 260, 200);
			SelectObject(hdc_mem[2], hBitmap);
			Boxfill(hdc_mem[2], 0, 0, 260, 200, hBrush_1); // 塗りつぶし
			if (!LoadPictureOnResource(hdc_mem[2], MAKEINTRESOURCE(IDG_IMGTOOLS)))
				ShowLoadPictureError(3);
			
			// ファイルアイコンを描画
			DrawFileImageEx(hdc_mem[2], TEXT("cmd.exe"), szSysDir, 4, 42, 134, 42);
			DrawFileImageEx(hdc_mem[2], TEXT("notepad.exe"), szSysDir, 4, 62, 134, 62);
			DrawFileImageEx(hdc_mem[2], TEXT("regedit.exe"), szWinDir, 4, 82, 134, 82);
			DrawFileImageEx(hdc_mem[2], TEXT("diskpart.exe"), szSysDir, 4, 102, 134, 102);
			
			// 文字を描画
			SelectObject(hdc_mem[2], hGuiFont);
			SetBkMode(hdc_mem[2], TRANSPARENT);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnReboot, 27, 5, 130, 17, 157, 5, 260, 17, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnShutdown, 27, 25, 130, 37, 157, 25, 260, 37, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnCommandPrompt, 27, 45, 130, 57, 157, 45, 260, 57, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnNotepad, 27, 65, 130, 77, 157, 65, 260, 77, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnRegEdit, 27, 85, 130, 97, 157, 85, 260, 97, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnDiskpart, 27, 105, 130, 117, 157, 105, 260, 117, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnLoadDriver, 27, 125, 130, 137, 157, 125, 260, 137, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnMSIME, 27, 145, 130, 157, 157, 145, 260, 157, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnWinRE, 27, 165, 130, 177, 157, 165, 260, 177, FALSE);
			DrawStringForImageButton(hdc_mem[2], langStr.szBtnInstallWindows, 27, 185, 130, 197, 157, 185, 260, 197, FALSE);
			
			break;
		
		case 4:
			{
				// 画像4 (apps)
				TCHAR szLaunchPath[MAX_PATH + 1];
				HBRUSH hBrush_2;
				hBrush_2 = CreateSolidBrush(RGB(236, 234, 164));
				
				hdc_mem[3] = CreateCompatibleDC(NULL);
				hBitmap = CreateCompatibleBitmap(hdc, 20, 300);
				SelectObject(hdc_mem[3], hBitmap);
				Boxfill(hdc_mem[3], 0, 0, 20, 300, hBrush_1); // 塗りつぶし
				if (!LoadPictureOnResource(hdc_mem[3], MAKEINTRESOURCE(IDG_IMGAPPS)))
					ShowLoadPictureError(4);
				
				SelectObject(hdc_mem[3], hBrush_1);
				
				// 枠を描画
				RoundRect(hdc_mem[3], 0, 60, 20, 80, 6, 6);
				SelectObject(hdc_mem[3], hBrush_2); // 色を変更
				RoundRect(hdc_mem[3], 0, 80, 20, 100, 6, 6);
				SelectObject(hdc_mem[3], hBrush_1); // 元に戻す
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
				
				// フォルダアイコンを描画
				DrawFolderImage(hdc_mem[3], 62, 82);
				
				// ファイルアイコンを描画
				DrawFileImage(hdc_mem[3], uaFilePath1, 2, 102);
				DrawFileImage(hdc_mem[3], uaFilePath2, 2, 122);
				DrawFileImage(hdc_mem[3], uaFilePath3, 2, 142);
				DrawFileImage(hdc_mem[3], uaFilePath4, 2, 162);
				DrawFileImage(hdc_mem[3], uaFilePath5, 2, 182);
				DrawFileImage(hdc_mem[3], uaFilePath6, 2, 202);
				DrawFileImage(hdc_mem[3], uaFilePath7, 2, 222);
				DrawFileImage(hdc_mem[3], uaFilePath8, 2, 242);
				DrawFileImage(hdc_mem[3], uaFilePath9, 2, 262);
				
				// USBメモリ内のランチャーのアイコンを取得 (取得できなかったらPStartのまま)
				for(int i = 1; i < 26; i++)
				{
					wsprintf(szLaunchPath, TEXT("%c:\\%s"), 'A'+i, ulPath);
					if (FileExist(szLaunchPath)) {
						// 塗りつぶしてから描画
						Boxfill(hdc_mem[3], 2, 42, 17, 57, hBrush_1);
						DrawFileImage(hdc_mem[3], szLaunchPath, 2, 42);
						break;
					}
				}
				
				// いろいろ削除
				DeleteObject(hBrush_2);
			}
			break;
		
		case 5:
			// 画像5
			hdc_mem[4] = CreateCompatibleDC(NULL);
			hBitmap = CreateCompatibleBitmap(hdc, 122, 80);
			SelectObject(hdc_mem[4], hBitmap);
			Boxfill(hdc_mem[4], 0, 0, 122, 80, hBrush_1); // 塗りつぶし
			if (!LoadPictureOnResource(hdc_mem[4], MAKEINTRESOURCE(IDG_IMGRES)))
				ShowLoadPictureError(5);
			
			// 文字を描画
			SelectObject(hdc_mem[4], hGuiFont);
			SetBkMode(hdc_mem[4], TRANSPARENT);
			DrawStringForImageButton(hdc_mem[4], TEXT("800x600"), 0, 3, 61, 15, 61, 3, 122, 15, TRUE);
			DrawStringForImageButton(hdc_mem[4], TEXT("1024x768"), 0, 19, 61, 31, 61, 19, 122, 31, TRUE);
			DrawStringForImageButton(hdc_mem[4], TEXT("1280x1024"), 0, 35, 61, 47, 61, 35, 122, 47, TRUE);
			DrawStringForImageButton(hdc_mem[4], TEXT("1920x1080"), 0, 51, 61, 63, 61, 51, 122, 63, TRUE);
			DrawStringForImageButton(hdc_mem[4], langStr.szBtnResChange, 15, 67, 61, 79, 61, 67, 105, 79, TRUE);
			
			break;
		
	}
	
	DeleteObject(hBitmap);
	DeleteObject(hBrush_1);
	ReleaseDC(hWnd, hdc);
}

void Controls::ShowLoadPictureError(int id)
{
	TCHAR szMes[156];
	wsprintf(szMes, TEXT("%s(%d)"), langStr.szErrorLoadPicture, id);
	MessageBox(NULL, szMes, langStr.szErrorTitle, MB_OK | MB_ICONERROR);
}

// コントロールのテキストを取得
int Controls::GetResText(TCHAR *lpResBuf, int flag)
{
	switch (flag)
	{
		case 1:
			SendMessage(hRes1, WM_GETTEXT, 6/*5(文字数) +1(NULL文字分)*/, (LPARAM)lpResBuf);
			break;
		
		case 2:
			SendMessage(hRes2, WM_GETTEXT, 6, (LPARAM)lpResBuf);
			break;
		
		default:
			return -1;
		
	}
	return 0;
}

// マウスイベント時のボタン描画処理の前処理
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
		if (!(lpControlInfo->option & 0x1))
			return;
		lpControlInfo->option &= ~0x1;
	}
	lpControlInfo->drawmode = flag;
	
	hdc_button = GetDC(hWnd_button);
	DrawImageButton(lpControlInfo, hdc_button);
	ReleaseDC(hWnd_button, hdc_button);
	
	return;
}

// オーナードロー時の描画処理の前処理
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
	RECT rect;
	
	// ボタンの大きさを取得
	GetClientRect(lpControlInfo->hWnd_control, &rect);
	
	// ボタンに画像を転送
	BitBlt(hdc_button, rect.left, rect.top, rect.right, rect.bottom, hdc_mem[lpControlInfo->hdc_id], lpControlInfo->imgpos[lpControlInfo->drawmode].x, lpControlInfo->imgpos[lpControlInfo->drawmode].y, SRCCOPY);
	
	if (lpControlInfo->option & 0x2) {
		// ボタン選択時 (キーボードフォーカス有り)
		InflateRect(&rect, -3, -3);
		DrawFocusRect(hdc_button, &rect);
	}
	
	return;
}

// TAbキーのフォーカス移動を処理
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
		if (hWnd != NULL && IsWindowEnabled(hWnd)) {
			if (lpControl->option & 0x10) {
				SendMessage(hWnd, EM_SETSEL, 0, -1);
			}
			SetFocus(hWnd);
			return;
		}
	}
}
