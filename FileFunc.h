// FileFunc.h

#pragma once

//�֐�
extern BOOL FileExist(LPCTSTR);
extern BOOL FileOpenDialog(HWND, LPCTSTR, LPCTSTR, TCHAR*);
extern HINSTANCE Execute(HWND, LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR, int);
extern void LoadIniData(void);
extern int FilePathToFileName(LPCTSTR, TCHAR*);

//��`
#define INI TEXT("./pecmd.ini")

// INI�t�@�C���p�ϐ�
extern TCHAR uNxPath[11], ulPath[_MAX_DIR + _MAX_FNAME + _MAX_EXT + 1], ufFilePath[MAX_PATH + 1], uaFilePath1[MAX_PATH + 1], uaFilePath2[MAX_PATH + 1], uaFilePath3[MAX_PATH + 1], uaFilePath4[MAX_PATH + 1], uaFilePath5[MAX_PATH + 1], uaFilePath6[MAX_PATH + 1], uaFilePath7[MAX_PATH + 1], uaFilePath8[MAX_PATH + 1], uaFilePath9[MAX_PATH + 1];
