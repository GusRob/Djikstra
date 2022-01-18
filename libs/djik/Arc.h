#pragma once

#include <iostream>
#include "Point.h"

struct Arc {
	std::string name;
	Point p1{};
	Point p2{};
	int id{};
	bool bi_dir{};
	bool inRoute = false;
	Arc();
	Arc(Point p1, Point p2, int id, bool bi_dir);
};

std::ostream &operator<<(std::ostream &os, const Arc &Arc);
