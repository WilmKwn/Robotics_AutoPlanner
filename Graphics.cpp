#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;

	writeFactory = NULL;
	textFormat = NULL;
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();

	if (writeFactory) writeFactory->Release();
}

void Graphics::Init(HWND wndHwnd)
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	RECT r;
	GetClientRect(wndHwnd, &r);
	factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(wndHwnd, D2D1::SizeU(r.right,r.bottom)), &renderTarget);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));

	SetBrushColor(0.0f, 0.0f, 1.0f);
}

void Graphics::DrawTexts(const wchar_t* text, int x, int y, float size, int length)
{
	std::wstring temp = text;
	std::string textString = std::string(temp.begin(), temp.end());
	int n = textString.length();

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &brush);
	writeFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", &textFormat);
	renderTarget->DrawTextA(text, sizeof(text) + length, textFormat, D2D1::RectF(x, y, x + (size * n), y + size), brush);
}

void Graphics::SetBrushColor(float r, float g, float b)
{
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b), &brush);
}

void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawLine(int x1, int y1, int x2, int y2)
{
	renderTarget->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), brush, 1.0f);
}

void Graphics::DrawRect(int x1, int y1, int x2, int y2)
{
	renderTarget->FillRectangle(D2D1::RectF(x1, y1, x2, y2), brush);
}

void Graphics::DrawCircle(int x, int y)
{
	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x,y), 6, 6), brush);
}

void Graphics::Begin()
{
	renderTarget->BeginDraw();
}

void Graphics::End()
{
	renderTarget->EndDraw();
}