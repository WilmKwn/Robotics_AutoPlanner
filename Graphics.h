#pragma once

#include <d2d1.h>
#include <dwrite.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "Window.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND wndHwnd);

	void SetBrushColor(float r, float g, float b);

	void ClearScreen(float r, float g, float b);

	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawRect(int x1, int y1, int x2, int y2);
	void DrawCircle(int x, int y);

	void DrawTexts(const wchar_t* text, int x, int y, float size, int length);

	void Begin();
	void End();

private:
	ID2D1Factory *factory;
	ID2D1HwndRenderTarget *renderTarget;
	ID2D1SolidColorBrush *brush;

	IDWriteFactory *writeFactory;
	IDWriteTextFormat *textFormat;
};