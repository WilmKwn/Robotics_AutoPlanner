#pragma once

#include "Input.h"
#include "Graphics.h"

#define FILENAME "C:\\Users\\wbi\\Documents\\Rowdy21\\src\\main\\include\\AutoInfo.h"

class Robot
{
public:
	Robot(Graphics *graphics, int x, int y);
	~Robot();

	int GetLength(POINT p1, POINT p2);
	float GetAngle(POINT p1, POINT p2);
	bool GetDir(POINT p1, POINT p2);

	void WriteToFile(POINT p1, POINT p2);
	void LastWrite();

	void DrawData();

	void Run();

private:
	Graphics *graphics;

	POINT pos;
	std::vector<POINT> prevPoints;

	bool leftClicked;
	bool rightClicked;
	bool pressed;

	std::ofstream writer;

	int startX;

	bool start;
	bool curveStarted;

	bool A;

	POINT curveEndPoint;

	std::vector<POINT> pathARed;
	std::vector<POINT> pathABlue;
	std::vector<POINT> pathBRed;
	std::vector<POINT> pathBBlue;
};