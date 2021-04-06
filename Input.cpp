#include "Input.h"

bool Input::leftClicked = false;
bool Input::rightClicked = false;

POINT Input::pos = {0,0};

bool Input::LeftClicked()
{
	return leftClicked;
}

void Input::SetLeftClicked(bool a)
{
	leftClicked = a;
}

bool Input::RightClicked()
{
	return rightClicked;
}

void Input::SetRightClicked(bool a)
{
	rightClicked = a;
}

bool Input::KeyDown(int vk_code)
{
	return (GetAsyncKeyState(vk_code) & 0x8000);
}

POINT Input::Pos()
{
	return pos;
}

void Input::SetPos(int x, int y)
{
	pos = { x, y };
}