#pragma once

#include <iostream>
#include <limits>

struct Point {
	std::string name;
	int x{};
	int y{};
	int id{};
	bool hover = false;
	bool selected = false;
	bool visited = false;
	float dist = std::numeric_limits<float>::infinity();
	Point();
	Point(int x, int y, int id);
	Point(std::string n, int x, int y, int id);
};

std::ostream &operator<<(std::ostream &os, const Point &Point);
