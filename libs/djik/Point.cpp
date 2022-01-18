#include "Point.h"

Point::Point() = default;
Point::Point(int initX, int initY, int initID) : x(initX), y(initY), id(initID) {}
Point::Point(std::string n, int initX, int initY, int initID) :
		name(std::move(n)),
		x(initX), y(initY), id(initID) {}

std::ostream &operator<<(std::ostream &os, const Point &Point) {
	os << Point.name << " ["
	   << Point.x << ", "
	   << Point.y << ", "
	   << Point.id << "]";
	return os;
}
