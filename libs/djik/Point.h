#pragma once

#include <iostream>

struct Point {
	std::string name;
	int x{};
	int y{};
	int id{};
	bool hover = false;
	bool selected = false;
	Point();
	Point(int x, int y, int id);
	Point(std::string n, int x, int y, int id);
};

std::ostream &operator<<(std::ostream &os, const Point &Point);
