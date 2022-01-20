#pragma once

#include <iostream>
#include <vector>
#include <Arc.h>
#include <Point.h>

struct Graph {
	std::vector<Arc> arcs{};
	std::vector<Point> points{};
	int routeMode = 0;
	Graph();
	std::vector<Arc *> arcsFrom(Point * p);
	std::vector<Arc *> arcsTo(Point * p);
	std::vector<Arc *> djikstra(Point *a, Point *b);
};
