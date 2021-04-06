#pragma once

#include "Graphics.h"

class Input
{
public:
	static bool LeftClicked();
	static void SetLeftClicked(bool a);

	static bool RightClicked();
	static void SetRightClicked(bool a);

	static bool KeyDown(int vk_code);

	static POINT Pos();
	static void SetPos(int x, int y);

private:
	static bool leftClicked;
	static bool rightClicked;

	static POINT pos;
};