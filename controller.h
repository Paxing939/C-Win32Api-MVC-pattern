#pragma once
#include <memory>
#include "view.h"
#include "model.h"

template<typename T>
class Controller {
public:

	explicit Controller(Model<T>* model) : view_(), model_(model) {}

	int Start(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
		UNREFERENCED_PARAMETER(hPrevInstance);
		UNREFERENCED_PARAMETER(lpCmdLine);

		if (!InitInstance(hInstance, nCmdShow))
		{
			return FALSE;
		}

		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY5));

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int)msg.wParam;
	}

	void Main(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam, HFONT hFont) {
		std::string str;
		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDM_ABOUT: // About
				view_.MakeDialog(hInst, hDlg, DialogType::About);
				break;
			case IDC_BUTTON_1: // Add
				view_.MakeDialog(hInst, hDlg, DialogType::Add);
				break;
			case IDC_BUTTON_3: // Delete
				view_.MakeDialog(hInst, hDlg, DialogType::Del);
				break;
			case IDC_BUTTON_4: // Get
				view_.MakeDialog(hInst, hDlg, DialogType::Get);
				break;
			case ID_GAME_EXIT:
			case WM_DESTROY:
				DeleteObject(hFont);
				PostQuitMessage(0);
				break;
			}
			break;
		case WM_PAINT:
			view_.Paint(hDlg, model_->GetOutIter(), model_->GetVisFirst(), model_->GetVisLast(), hFont);
			break;
		}
	}

	void Add(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		GetFirstVisitor<std::string> v_first;
		GetLastVisitor<std::string> v_last;
		switch (LOWORD(wParam)) {
		case IDOK:
			model_->Add(atoi(view_.GetText(hDlg, IDC_EDIT2).c_str()), atof(view_.GetText(hDlg, IDC_EDIT1).c_str()));
			model_->UpdateStrings();
		case WM_DESTROY:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}

	void Del(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		GetFirstVisitor<std::string> v_first;
		GetLastVisitor<std::string> v_last;
		switch (LOWORD(wParam)) {
		case IDOK:
			model_->Delete(atoi(view_.GetText(hDlg, IDC_EDIT1).c_str()));
			model_->UpdateStrings();
		case WM_DESTROY:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}

	void Get(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		T tmp;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_1:
			tmp = model_->Get(atoi(view_.GetText(hDlg, IDC_EDIT1).c_str()));
			view_.PrintText(hDlg, IDC_STATIC1, (std::to_string(tmp)).c_str());
			break;
		case WM_DESTROY:
		case IDC_BUTTON_2:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}

	void About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (LOWORD(wParam)) {
		case WM_DESTROY:
		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
	}

	const static HFONT hFont;

private:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
		hInst = hInstance;

		HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgMain);

		if (!hWnd)
		{
			return FALSE;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		return TRUE;
	}

	View view_;
	Model<T>* model_;
};

template<typename T>
const HFONT Controller<T>::hFont = CreateFont(20, 0, 0, 0,
	FW_DONTCARE,
	FALSE, FALSE, FALSE, // курсив подчеркивание зачеркивание (прост поставить true)
	DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
	VARIABLE_PITCH, TEXT("Areal")); // в ковычках можно указать какой-нибудь шрифт
