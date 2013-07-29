// lang.cpp : åæåÍånÇÃä÷êîÇíÒãü

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "lang/resource-lang.h"

#include "lang.h"
#include "FileFunc.h"
#include "pecmd.h"

LANGSTRING langStr;

int LoadLangFile(int IsFileDialog)
{
	TCHAR szLangFilePath[MAX_PATH + 1];
	HMODULE hLangFile;
	
	if (FileExist(TEXT("pecmd-lang.dll"))) {
		lstrcpy(szLangFilePath, TEXT("pecmd-lang.dll"));
	} else {
		GetPrivateProfileString(TEXT("Language"), TEXT("LangFile"), NULL, szLangFilePath, MAX_PATH, INI);
	}
	
	if (FileExist(szLangFilePath)) {
		hLangFile = LoadLibraryEx(szLangFilePath, NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_WITH_ALTERED_SEARCH_PATH);
	} else {
		hLangFile = GetModuleHandle(NULL);
	}
	
	LoadString(hLangFile, IDS_APPLICATIONERROR, langStr.szAppErrorTitle, 40);
	LoadString(hLangFile, IDS_ERROR_COM, langStr.szErrorCOM, 150);
	LoadString(hLangFile, IDS_OPEN_ALLFILES, langStr.szAllFiles, 50);
	
	if (IsFileDialog) {
		wsprintf(langStr.szAllFiles, TEXT("%s (*.*)@*.*@@"), langStr.szAllFiles);
		
		for(int i = 0; i < 51; i++)
		{
			if (langStr.szAllFiles[i] == TEXT('@')) {
				langStr.szAllFiles[i] = TEXT('\0');
			}
		}
		return 0;
	} else {
		LoadString(hLangFile, IDS_OPEN_DRIVERINFORMATION, langStr.szDrvLoadInfo, 50);
		wsprintf(langStr.szDrvLoadInfo, TEXT("%s (*.inf)@*.inf@%s (*.*)@*.*@@"), langStr.szDrvLoadInfo, langStr.szAllFiles);
		
		for(int i = 0; i < 132; i++)
		{
			if (langStr.szDrvLoadInfo[i] == TEXT('@')) {
				langStr.szDrvLoadInfo[i] = TEXT('\0');
			}
		}
	}
	
	LoadString(hLangFile, IDS_TITLE, langStr.szTitle, 50);
	LoadString(hLangFile, IDS_SWITCHPROGRAMS, langStr.szSwitchPrograms, 80);
	
	LoadString(hLangFile, IDS_ERROR, langStr.szErrorTitle, 40);
	LoadString(hLangFile, IDS_INFORMATION, langStr.szInformationTitle, 40);
	
	LoadString(hLangFile, IDS_ERROR_WNDCLASS, langStr.szErrorWndClass, 150);
	LoadString(hLangFile, IDS_ERROR_CREATEWINDOW, langStr.szErrorCreateWindow, 150);
	LoadString(hLangFile, IDS_ERROR_GETMSG, langStr.szErrorGetMsg, 150);
	LoadString(hLangFile, IDS_ERROR_LOADPICTURE, langStr.szErrorLoadPicture, 150);
	LoadString(hLangFile, IDS_ERROR_FILENOTFOUND, langStr.szErrorFileNotFound, 150);
	LoadString(hLangFile, IDS_ERROR_SHUTDOWN, langStr.szErrorShutdown, 150);
	
	LoadString(hLangFile, IDS_SHUTDOWN, langStr.szShutdown, 150);
	LoadString(hLangFile, IDS_SHUTDOWN_TITLE, langStr.szShutdownTitle, 40);
	
	LoadString(hLangFile, IDS_RES_FAILEDGETDISPINFO, langStr.szResFailedGetDispInfo, 150);
	LoadString(hLangFile, IDS_RES_RESTART, langStr.szResRestart, 150);
	LoadString(hLangFile, IDS_RES_BADFLAGS, langStr.szResBadFlags, 150);
	LoadString(hLangFile, IDS_RES_BADPARAM, langStr.szResBadParam, 150);
	LoadString(hLangFile, IDS_RES_FAILED, langStr.szResFailed, 150);
	LoadString(hLangFile, IDS_RES_BADMODE, langStr.szResBadMode, 150);
	
	LoadString(hLangFile, IDS_BUTTON_7ZIP, langStr.szBtn7Zip, 50);
	LoadString(hLangFile, IDS_BUTTON_COMPUTER, langStr.szBtnComputer, 50);
	LoadString(hLangFile, IDS_BUTTON_QDIR, langStr.szBtnQDir, 50);
	LoadString(hLangFile, IDS_BUTTON_USERSFILER, langStr.szBtnUsersFiler, 50);
	
	LoadString(hLangFile, IDS_BUTTON_REBOOT, langStr.szBtnReboot, 50);
	LoadString(hLangFile, IDS_BUTTON_SHUTDOWN, langStr.szBtnShutdown, 50);
	LoadString(hLangFile, IDS_BUTTON_COMMANDPROMPT, langStr.szBtnCommandPrompt, 50);
	LoadString(hLangFile, IDS_BUTTON_NOTEPAD, langStr.szBtnNotepad, 50);
	LoadString(hLangFile, IDS_BUTTON_REGEDIT, langStr.szBtnRegEdit, 50);
	LoadString(hLangFile, IDS_BUTTON_DISKPART, langStr.szBtnDiskpart, 50);
	LoadString(hLangFile, IDS_BUTTON_LOADDRIVER, langStr.szBtnLoadDriver, 50);
	LoadString(hLangFile, IDS_BUTTON_MSIME, langStr.szBtnMSIME, 50);
	LoadString(hLangFile, IDS_BUTTON_WINDOWSRE, langStr.szBtnWinRE, 50);
	LoadString(hLangFile, IDS_BUTTON_INSTALLWINDOWS, langStr.szBtnInstallWindows, 50);
	
	LoadString(hLangFile, IDS_BUTTON_TRUEIMAGE, langStr.szBtnTrueImage, 50);
	LoadString(hLangFile, IDS_BUTTON_DISCWIZARD, langStr.szBtnDiscWizard, 50);
	LoadString(hLangFile, IDS_BUTTON_PARAGONBACKUPANDRECOVERY, langStr.szBtnParagonBackupAndRecovery, 50);
	LoadString(hLangFile, IDS_BUTTON_EASEUSTODOBACKUP, langStr.szBtnEaseUSTodoBackup, 50);
	LoadString(hLangFile, IDS_BUTTON_AOMEIBACKUPPER, langStr.szBtnAomeiBackupper, 50);
	LoadString(hLangFile, IDS_BUTTON_EASEUSPARTITIONMASTER, langStr.szBtnEaseUSPartitionMaster, 50);
	LoadString(hLangFile, IDS_BUTTON_MINITOOLPARTITIONWIZARD, langStr.szBtnMiniToolPartitionWizard, 50);
	LoadString(hLangFile, IDS_BUTTON_ACRONISDISKDIRECTOR, langStr.szBtnAcronisDiskDirector, 50);
	LoadString(hLangFile, IDS_BUTTON_PARAGONPARTITIONMANAGER, langStr.szBtnParagonPartitionManager, 50);
	LoadString(hLangFile, IDS_BUTTON_AOMEIPARTITIONASSISTANT, langStr.szBtnAomeiPartitionAssistant, 50);
	LoadString(hLangFile, IDS_BUTTON_OTHERDISKTOOL, langStr.szBtnOtherDiskTool, 50);
	
	LoadString(hLangFile, IDS_BUTTON_RESCHANGE, langStr.szBtnResChange, 50);
	
	lstrcat(langStr.szBtnQDir, TEXT(" ]"));
	
	FreeLibrary(hLangFile);
	
	return 0;
}
