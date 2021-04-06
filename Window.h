#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windowsx.h>
#include "Input.h"

#define START_LINE_X 76.2

#define FIELD_LENGTH 915
#define FIELD_HEIGHT 458

class Window
{
public:
	Window();
	~Window();

	void Init(int width, int height, HINSTANCE hInstance);

	HWND GetHandle();

private:
	HWND m_handle;
};