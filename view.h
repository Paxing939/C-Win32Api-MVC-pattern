#pragma once
#include "5.h"
#include "stdafx.h"

extern HINSTANCE hInst;
INT_PTR CALLBACK	DlgMain(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgAdd(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgDel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgGet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

enum class DialogType { Add, Set, Get, Del, About };

class View {
public:
	void Paint(HWND hDlg, std::string str_to_out_iter, std::string str_to_first, std::string str_to_last, HFONT hFont);

	void MakeDialog(HINSTANCE hInst, HWND hDlg, DialogType type);

	void PrintText(HWND hDlg, int ID, const char* outBuff);

	void Line(HDC hdc, int X0, int Y0, int X, int Y);

	std::string GetText(HWND hDlg, int ID);
};