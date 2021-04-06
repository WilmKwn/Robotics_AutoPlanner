#include "Window.h"
#include "Graphics.h"
#include "Robot.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	Window *window = new Window();
	window->Init(FIELD_LENGTH, FIELD_HEIGHT, hInstance);

	Graphics *graphics = new Graphics();
	graphics->Init(window->GetHandle());

	Robot *robot = new Robot(graphics, START_LINE_X, 100);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		graphics->Begin();
		graphics->ClearScreen(0.6f, 0.6f, 0.6f);

		// START LINE
		graphics->SetBrushColor(0.0f, 0.0f, 1.0f);
		graphics->DrawLine(START_LINE_X, 0, START_LINE_X, FIELD_HEIGHT);

		graphics->SetBrushColor(1.0f, 0.0f, 0.0f);
		graphics->DrawLine(FIELD_LENGTH - START_LINE_X, 0, FIELD_LENGTH - START_LINE_X, FIELD_HEIGHT);

		robot->Run();

		graphics->End();
	}
	robot->LastWrite();
	return msg.wParam;
}