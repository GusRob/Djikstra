#pragma once

#include <iostream>

struct Button {
	int xPos;
	int yPos;
	int width;
	int height;
	void (* funcOnClick)();
	std::string bgIm;
	int imWidthPad;

	bool selected = false;
	bool hover = false;

	Button();
	Button(void (* OnClick)(), int x, int y, int w, int h, int wp, std::string fp);

	bool IsWithin(int x, int y);
	void Click();
	void UnClick();
	void ToggleHover();
};
