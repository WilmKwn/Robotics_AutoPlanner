#include "Window.h"

Window::Window()
{
	m_handle = NULL;
}

Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_MOUSEMOVE:
		Input::SetPos(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
		return 0;
	case WM_LBUTTONDOWN:
		Input::SetLeftClicked(true);
		return 0;
	case WM_LBUTTONUP:
		Input::SetLeftClicked(false);
		return 0;
	case WM_RBUTTONDOWN:
		Input::SetRightClicked(true);
		return 0;
	case WM_RBUTTONUP:
		Input::SetRightClicked(false);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void Window::Init(int width, int height, HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "o";
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.lpfnWndProc = WndProc;
	RegisterClassEx(&wcex);

	m_handle = CreateWindowEx(NULL, "o", "ok", WS_POPUPWINDOW | WS_VISIBLE, 100, 100, width, height, NULL, NULL, hInstance, NULL);
}

HWND Window::GetHandle()
{
	return m_handle;
}