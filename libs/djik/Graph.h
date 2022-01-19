#pragma once

#include <iostream>
#include <vector>
#include <Arc.h>
#include <Point.h>

struct Graph {
  std::vector<Arc> arcs{};
  std::vector<Point> points{};
	Graph();

  std::vector<Arc> djikstra(Point a, Point b);
};
