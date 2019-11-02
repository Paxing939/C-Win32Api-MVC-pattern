#include "stdafx.h"
#include "5.h"
#include <ctime>
#include <iterator>
#include "controller.h"
#include "model.h"

using std::vector;
using std::wstring;
using std::to_wstring;

#define MAX_LOADSTRING 100

HINSTANCE hInst;
INT_PTR CALLBACK	DlgMain(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgAdd(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgDel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgGet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

Polynomial<float> v;
Controller<float> controller(&v);

// Без этих функций WinApi не запускается
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
	return controller.Start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

INT_PTR CALLBACK DlgMain(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
	case WM_PAINT:
		controller.Main(hDlg, message, wParam, lParam, Controller<std::string>::hFont);
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgAdd(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		controller.Add(hDlg, message, wParam, lParam);
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgDel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		controller.Del(hDlg, message, wParam, lParam);
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgGet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		controller.Get(hDlg, message, wParam, lParam);
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		controller.About(hDlg, message, wParam, lParam);
		break;
	}
	return (INT_PTR)FALSE;
}