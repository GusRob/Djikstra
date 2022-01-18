#pragma once

#include <iostream>

struct Arc {
	std::string name;
	int p1{};
	int p2{};
	int id{};
	bool bi_dir{};
	Arc();
	Arc(int p1, int p2, int id, bool bi_dir);
};

std::ostream &operator<<(std::ostream &os, const Arc &Arc);
