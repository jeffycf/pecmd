// graphic.h

#pragma once

// �֐�
extern BOOL LoadPictureOnResource(HDC, LPCTSTR, LPCTSTR);
extern void DrawFolderImage(HDC, int, int);
extern void DrawFileImage(HDC, LPCTSTR, int, int);
extern void Boxfill(HDC, int, int, int, int, HBRUSH);

