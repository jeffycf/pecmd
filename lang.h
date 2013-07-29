// lang.h

#pragma once

typedef struct LANGSTRING
{
	TCHAR szTitle[51];
	TCHAR szDrvLoadInfo[132];
	TCHAR szAllFiles[66];
	TCHAR szSwitchPrograms[81];

	TCHAR szErrorTitle[41];
	TCHAR szAppErrorTitle[41];
	TCHAR szInformationTitle[41];

	TCHAR szErrorCOM[151];
	TCHAR szErrorWndClass[151];
	TCHAR szErrorCreateWindow[151];
	TCHAR szErrorGetMsg[151];
	TCHAR szErrorLoadPicture[151];
	TCHAR szErrorFileNotFound[151];
	TCHAR szErrorShutdown[151];

	TCHAR szShutdown[151];
	TCHAR szShutdownTitle[41];

	TCHAR szResFailedGetDispInfo[151];
	TCHAR szResRestart[151];
	TCHAR szResBadFlags[151];
	TCHAR szResBadParam[151];
	TCHAR szResFailed[151];
	TCHAR szResBadMode[151];
	TCHAR szResNotUpdated[151];

	TCHAR szBtn7Zip[51];
	TCHAR szBtnComputer[51];
	TCHAR szBtnQDir[51];
	TCHAR szBtnUsersFiler[51];

	TCHAR szBtnReboot[51];
	TCHAR szBtnShutdown[51];
	TCHAR szBtnCommandPrompt[51];
	TCHAR szBtnNotepad[51];
	TCHAR szBtnRegEdit[51];
	TCHAR szBtnDiskpart[51];
	TCHAR szBtnLoadDriver[51];
	TCHAR szBtnMSIME[51];
	TCHAR szBtnWinRE[51];
	TCHAR szBtnInstallWindows[51];

	TCHAR szBtnTrueImage[51];
	TCHAR szBtnDiscWizard[51];
	TCHAR szBtnParagonBackupAndRecovery[51];
	TCHAR szBtnEaseUSTodoBackup[51];
	TCHAR szBtnAomeiBackupper[51];
	TCHAR szBtnEaseUSPartitionMaster[51];
	TCHAR szBtnMiniToolPartitionWizard[51];
	TCHAR szBtnAcronisDiskDirector[51];
	TCHAR szBtnParagonPartitionManager[51];
	TCHAR szBtnAomeiPartitionAssistant[51];
	TCHAR szBtnOtherDiskTool[51];

	TCHAR szBtnResChange[51];

} LANGSTRING;

extern LANGSTRING langStr;

extern int LoadLangFile(int);
