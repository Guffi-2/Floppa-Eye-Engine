#pragma once

/// <summary>
/// WndProc
/// </summary>
/// <param name="hwndWND"></param>
/// <param name="msgWND"></param>
/// <param name="wpWND"></param>
/// <param name="lpWND"></param>
/// <returns></returns>
static LRESULT WINAPI WndProc(HWND hwndWND, UINT msgWND, WPARAM wpWND, LPARAM lpWND) {

	switch (msgWND)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hwndWND, msgWND, wpWND, lpWND);
}

/// <summary>
/// Window
/// </summary>
struct eWindow {
	HWND hWnd;
	WNDCLASSEXA wndClassExA;
	void eCreateWindow() {
		int xScreen = GetSystemMetrics(SM_CXSCREEN);
		int yScreen = GetSystemMetrics(SM_CYSCREEN);
		ZeroMemory(&wndClassExA, sizeof(wndClassExA));
		wndClassExA.cbSize = sizeof(WNDCLASSEXA);
		wndClassExA.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClassExA.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClassExA.lpszClassName = "FloppaClass";
		wndClassExA.lpfnWndProc = WndProc;
		RegisterClassExA(&wndClassExA);
		hWnd = CreateWindowEx(NULL, wndClassExA.lpszClassName, "FloppaEye", WS_OVERLAPPEDWINDOW, (xScreen - 1280) / 2, (yScreen - 700) / 2, 1280, 700, NULL, NULL, NULL, NULL); //Создание окна
	}
	void eOpenWindow() {
		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}
	void eUpdateWindow() {
		UpdateWindow(hWnd);
	}
	void eRelease() {
		DestroyWindow(hWnd);
		UnregisterClass(wndClassExA.lpszClassName, wndClassExA.hInstance);
	}
};

