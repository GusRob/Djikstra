#pragma once

#include <iostream>
#include "Point.h"

struct Arc {
	std::string name;
	Point *p1{};
	Point *p2{};
	int id{};
	bool bi_dir{};
	float preloaded_length = 1;
	bool inRoute = false;
	Arc();
	Arc(Point *p1, Point *p2, int id, bool bi_dir, int len);
	float length(int mode);
};

std::ostream &operator<<(std::ostream &os, const Arc &Arc);
