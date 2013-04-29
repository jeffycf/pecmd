// graphic.h

#pragma once

// ä÷êî
extern BOOL LoadPictureOnResource(HDC, LPCTSTR);
extern void DrawFolderImage(HDC, int, int);
extern void DrawFileImage(HDC, LPCTSTR, int, int);
extern void DrawFileImageEx(HDC, LPCTSTR, LPCTSTR, int, int, int, int);
extern void Boxfill(HDC, int, int, int, int, HBRUSH);
extern void DrawStringForImageButton(HDC, LPCTSTR, int, int, int, int, int, int, int, int, BOOL);
