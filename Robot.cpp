#include "Robot.h"

Robot::Robot(Graphics *graphics, int x, int y)
{
	this->graphics = graphics;
	
	startX = x;

	pos = { x, y };

	leftClicked = rightClicked = pressed = false;

	this->startX = startX;

	start = false;

	curveStarted = false;

	A = true;

	// PATH A
	pathARed.push_back({ 229, 229 });
	pathARed.push_back({ 381, 305 });
	pathARed.push_back({ 457, 76 });

	pathABlue.push_back({ 457, 381 });
	pathABlue.push_back({ 533, 152 });
	pathABlue.push_back({ 686, 229 });

	// PATH B
	pathBRed.push_back({ 229,152 });
	pathBRed.push_back({ 381,305 });
	pathBRed.push_back({ 533,152 });

	pathBBlue.push_back({ 457,305 });
	pathBBlue.push_back({ 610,152 });
	pathBBlue.push_back({ 762,305 });

	writer.open(FILENAME);
	writer << "#pragma once\n";
	writer << "#include <vector>\n";
	writer << "class AutoInfo {\n";
	writer << "public:\n";
	writer << "class MoveInfo {\n";
	writer << "public:\n";
	writer << "MoveInfo(bool dir, float angle, float distance) {\n";
	writer << "this->dir = dir;\n";
	writer << "this->angle = angle;\n";
	writer << "this->distance = distance;\n";
	writer << "}\n";
	writer << "bool dir;\n";
	writer << "float angle;\n";
	writer << "float distance;\n";
	writer << "};\n";
	writer << "std::vector<MoveInfo*> list = {\n";
	writer.close();
}

Robot::~Robot()
{
}

void Robot::LastWrite()
{
	writer.open(FILENAME, std::ios_base::app);
	writer << "};\n";
	writer << "};";
}

int Robot::GetLength(POINT p1, POINT p2)
{
	return (int)sqrt(pow((p2.x-p1.x), 2) + pow((p2.y-p1.y), 2));
}

float Robot::GetAngle(POINT p1, POINT p2)
{
	if (GetLength(p1, p2) == 0) return 0.0f;

	float hypotenuse = (float)GetLength(p1, p2);
	float adjacent = fabs(p2.x - p1.x);
	
	POINT p0;
	if (prevPoints.size() > 1) p0 = prevPoints.at(prevPoints.size() - 2);
	else return (acosf(adjacent / hypotenuse) * (180.0 / 3.14159));

	if (p1.x - p0.x > -1 && p1.x - p0.x < 1) {
		float opposite = adjacent;
		float angle = (asinf(opposite / hypotenuse) * (180.0 / 3.14159));
		return (p1.y < p0.y) ? ((p2.y > p1.y) ? 90.0f + angle : angle) : ((p2.y < p1.y) ? 90.0f + angle : angle);
	}

	float slope0 = (float)(p1.y - p0.y) / (float)(p1.x - p0.x);
	float yint0 = p1.y - slope0 * p1.x;

	if (slope0 == 0) {
		float angle = (acosf(adjacent / hypotenuse) * (180.0 / 3.14159));
		return (p1.x > p0.x) ? ((p2.x < p1.x) ? 90.0f + angle : angle) : ((p2.x > p1.x) ? 90.0f + angle : angle);
	}

	float slope0Perp = -1.0f / slope0;
	float yint0Perp = p1.y - slope0Perp * p1.x;

	float newYint = p2.y - slope0 * p2.x;

	float x = (float)(yint0Perp - newYint) / (float)(slope0 - slope0Perp);
	float y = slope0 * x + newYint;
	float opposite = GetLength(p2, { (int)x, (int)y });

	bool behind = (p1.y < p0.y) ? (p2.y > p2.x * slope0Perp + yint0Perp) : (p2.y < p2.x * slope0Perp + yint0Perp);
	float angle = (asinf(opposite / hypotenuse) * (180.0f / 3.14f));

	if (behind) return 90.0f + angle;
	else return 90.0f - angle;
}

bool Robot::GetDir(POINT p1, POINT p2)
{
	POINT p0;
	if (prevPoints.size()>1) p0 = prevPoints.at(prevPoints.size()-2);
	else return (p2.y > p1.y);

	if (p1.x - p0.x > -1 && p1.x - p0.x < 1) {
		return (p1.y < p0.y) ? (p2.x > p1.x) : (p2.x < p1.x);
	}
	float slope0 = (float)(p1.y - p0.y) / (float)(p1.x - p0.x);
	float yint0 = p1.y - slope0 * p1.x;

	return (p1.x > p0.x) ? (p2.y > p2.x*slope0 + yint0) : (p2.y < p2.x*slope0 + yint0);
}

void Robot::WriteToFile(POINT p1, POINT p2)
{
	writer.open(FILENAME, std::ios_base::app);
	
	std::stringstream ss;
	ss << "new MoveInfo(";
	ss << (GetDir(p1, p2) ? "true" : "false") << ",";
	ss << (int)GetAngle(p1, p2) << ",";
	ss << GetLength(p1, p2);
	ss << "),";

	writer << ss.str() << "\n";
	writer.close();
}

void Robot::DrawData() 
{
	std::stringstream textSS;
	textSS << "Distance (cm) : " << GetLength(prevPoints.back(), Input::Pos());
	std::string t = textSS.str();
	std::wstring text(t.begin(), t.end());
	graphics->DrawTexts(text.c_str(), FIELD_LENGTH / 2 + 100, 0, 20.0f, (t.size() > 17) ? 15 : 14);

	textSS.str(std::string());
	textSS << "Angle (deg) : " << (int)GetAngle(prevPoints.back(), Input::Pos());
	t = textSS.str();
	text = std::wstring(t.begin(), t.end());
	graphics->DrawTexts(text.c_str(), FIELD_LENGTH / 2 + 100, 20, 20.0f, (t.size() > 16) ? 13 : 12);
}

void Robot::Run()
{
	int size = 71;
	if (!start) {
		graphics->SetBrushColor(0.5f, 0.0f, 0.0f);
		graphics->DrawRect(startX - size, Input::Pos().y - size / 2, startX, Input::Pos().y + size / 2);

		std::stringstream textSS;
		textSS << "Distance (cm) : " << Input::Pos().y;
		std::string t = textSS.str();
		std::wstring text(t.begin(), t.end());
		graphics->DrawTexts(text.c_str(), FIELD_LENGTH / 2 + 100, 0, 20.0f, (t.size() > 17) ? 15 : 14);

		if (Input::LeftClicked() && Input::LeftClicked) {
			pos.y = Input::Pos().y;
			prevPoints.push_back(pos);
			leftClicked = true;
			start = true;
		}
		else if (!Input::LeftClicked()) {
			leftClicked = false;
		}
	}
	else {
		// left click
		if (Input::LeftClicked() && Input::Pos().x > startX && !leftClicked && !curveStarted) {
			if ((Input::Pos().x == prevPoints.back().x && Input::Pos().y == prevPoints.back().y));
			else WriteToFile(pos, Input::Pos());

			pos.x = Input::Pos().x;
			pos.y = Input::Pos().y;

			prevPoints.push_back(pos);

			leftClicked = true;
		}
		else if (!Input::LeftClicked()) leftClicked = false;

		// right click
		if (Input::RightClicked() && Input::Pos().x > startX && !rightClicked && !curveStarted) {
			curveEndPoint = Input::Pos();
			curveStarted = true;
			rightClicked = true;
		}
		else if (!Input::RightClicked()) rightClicked = false;

		graphics->SetBrushColor(0.0f, 0.0f, 0.0f);

		if (curveStarted) {
			POINT p0 = prevPoints.back();
			POINT v = Input::Pos();

			if (Input::RightClicked() && Input::Pos().x > startX && !rightClicked) {

				curveStarted = false;
			}
		}
		else graphics->DrawLine(pos.x, pos.y, Input::Pos().x, Input::Pos().y);

		for (int i = 0; i < prevPoints.size() - 1; i++) {
			graphics->DrawLine(prevPoints[i].x, prevPoints[i].y, prevPoints[i + 1].x, prevPoints[i + 1].y);
		}

		DrawData();

		graphics->SetBrushColor(0.5f, 0.0f, 0.0f);
		graphics->DrawRect(startX - size, prevPoints.at(0).y - size / 2, startX, prevPoints.at(0).y + size / 2);
	}

	if (Input::KeyDown(VK_SPACE) && !pressed) {
		A = A ? 0 : 1;
		pressed = true;
	}
	else if (!Input::KeyDown(VK_SPACE)) pressed = false;

	std::vector<POINT> path;
	for (int i = 0; i < pathARed.size(); i++) {
		graphics->SetBrushColor(1.0f, 0.0f, 0.0f);
		path = A ? pathARed : pathBRed;
		graphics->DrawCircle(path.at(i).x, path.at(i).y);
		graphics->SetBrushColor(0.0f, 0.0f, 1.0f);
		path = A ? pathABlue : pathBBlue;
		graphics->DrawCircle(path.at(i).x, path.at(i).y);
	}
}