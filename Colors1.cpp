// Colors1.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Colors1.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK ScrollProc(HWND, UINT, WPARAM, LPARAM);
int idFocus;
WNDPROC OldScroll[3];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COLORS1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COLORS1));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COLORS1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COLORS1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static COLORREF crPrim[3] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
	static HBRUSH hBrush[3], hBrushStatic;
	static HWND hwndScroll[3], hwndLabel[3], hwndValue[3],
		hwndRect;
	static int color[3], cyChar;
	static RECT rcColor;
	static const TCHAR * szColorLabel[] = { TEXT("Red"), TEXT("Green"), TEXT("Blue") };
	HINSTANCE hInstance;
	int i, cxClient, cyClient;
	TCHAR szBuffer[10];

    switch (message)
    {
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

		// Create the white-rectangle window against which the
		// scroll bars will be positioned. The child window ID is 9.
		hwndRect = CreateWindow(TEXT("static"), NULL,
			WS_CHILD | WS_VISIBLE | SS_WHITERECT,
			0, 0, 0, 0,
			hWnd, (HMENU)9, hInstance, NULL);

		for (i = 0; i < 3; i++)
		{
			// The three scroll bars have IDs 0, 1, and 2, with
			// scroll bar ranges from 0 through 255.

			hwndScroll[i] = CreateWindow(TEXT("scrollbar"), NULL,
				WS_CHILD | WS_VISIBLE |
				WS_TABSTOP | SBS_VERT,
				0, 0, 0, 0,
				hWnd, (HMENU)i, hInstance, NULL);

			SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
			SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

			// The three color-name labels have IDs 3, 4, and 5,
			// and text strings "Red", "Green", and "Blue".

			hwndLabel[i] = CreateWindow(TEXT("static"), szColorLabel[i],
				WS_CHILD | WS_VISIBLE | SS_CENTER,
				0, 0, 0, 0,
				hWnd, (HMENU)(i + 3),
				hInstance, NULL);

			// The three color-value text fields have IDs 6, 7,
			// and 8, and initial text strings of "0".

			hwndValue[i] = CreateWindow(TEXT("static"), TEXT("0"),
				WS_CHILD | WS_VISIBLE | SS_CENTER,
				0, 0, 0, 0,
				hWnd, (HMENU)(i + 6),
				hInstance, NULL);

			OldScroll[i] = (WNDPROC)SetWindowLong(hwndScroll[i],
				GWL_WNDPROC, (LONG)ScrollProc);

			hBrush[i] = CreateSolidBrush(crPrim[i]);
		}

		hBrushStatic = CreateSolidBrush(
			GetSysColor(COLOR_BTNHIGHLIGHT));

		cyChar = HIWORD(GetDialogBaseUnits());
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		SetRect(&rcColor, cxClient / 2, 0, cxClient, cyClient);

		MoveWindow(hwndRect, 0, 0, cxClient / 2, cyClient, TRUE);

		for (i = 0; i < 3; i++)
		{
			MoveWindow(hwndScroll[i],
				(2 * i + 1) * cxClient / 14, 2 * cyChar,
				cxClient / 14, cyClient - 4 * cyChar, TRUE);

			MoveWindow(hwndLabel[i],
				(4 * i + 1) * cxClient / 28, cyChar / 2,
				cxClient / 7, cyChar, TRUE);

			MoveWindow(hwndValue[i],
			(4 * i + 1) * cxClient / 28,
				cyClient - 3 * cyChar / 2,
				cxClient / 7, cyChar, TRUE);
		}
		SetFocus(hWnd);
		return 0;

	case WM_SETFOCUS:
		SetFocus(hwndScroll[idFocus]);
		return 0;

	case WM_VSCROLL:
		i = GetWindowLong((HWND)lParam, GWL_ID);

		switch (LOWORD(wParam))
		{
		case SB_PAGEDOWN:
			color[i] += 15;
			// fall through
		case SB_LINEDOWN:
			color[i] = min(255, color[i] + 1);
			break;

		case SB_PAGEUP:
			color[i] -= 15;
			// fall through
		case SB_LINEUP:
			color[i] = max(0, color[i] - 1);
			break;
		case SB_TOP:
			color[i] = 0;
			break;

		case SB_BOTTOM:
			color[i] = 255;
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			color[i] = HIWORD(wParam);
			break;

		default:
			break;
		}
		SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
		wsprintf(szBuffer, TEXT("%i"), color[i]);
		SetWindowText(hwndValue[i], szBuffer);

		DeleteObject((HBRUSH)
			SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG)
				CreateSolidBrush(RGB(color[0], color[1], color[2]))));

		InvalidateRect(hWnd, &rcColor, TRUE);
		return 0;

	case WM_CTLCOLORSCROLLBAR:
		i = GetWindowLong((HWND)lParam, GWL_ID);
		return (LRESULT)hBrush[i];

	case WM_CTLCOLORSTATIC:
		i = GetWindowLong((HWND)lParam, GWL_ID);

		if (i >= 3 && i <= 8) // static text controls
		{
			SetTextColor((HDC)wParam, crPrim[i % 3]);
			SetBkColor((HDC)wParam, GetSysColor
			(COLOR_BTNHIGHLIGHT));
			return (LRESULT)hBrushStatic;
		}
		break;
	case WM_SYSCOLORCHANGE:
		DeleteObject(hBrushStatic);
		hBrushStatic = CreateSolidBrush
		(GetSysColor(COLOR_BTNHIGHLIGHT));
		return 0;
    case WM_DESTROY:
		DeleteObject((HBRUSH)
			SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG)
				GetStockObject(WHITE_BRUSH)));

		for (i = 0; i < 3; i++)
			DeleteObject(hBrush[i]);

		DeleteObject(hBrushStatic);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK ScrollProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	int id = GetWindowLong(hwnd, GWL_ID);
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_TAB)
			SetFocus(GetDlgItem(GetParent(hwnd),
			(id + (GetKeyState(VK_SHIFT) < 0 ? 2 : 1)) % 3));
		break;
	case WM_SETFOCUS:
		idFocus = id;
		break;
	}
	return CallWindowProc(OldScroll[id], hwnd, message, wParam, lParam);
}
