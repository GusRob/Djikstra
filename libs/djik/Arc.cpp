#include "Arc.h"

Arc::Arc() = default;
Arc::Arc(int initP1, int initP2, int initID, bool initDir) : p1(initP1), p2(initP2), id(initID), bi_dir(initDir) {}

std::ostream &operator<<(std::ostream &os, const Arc &Arc) {
	os << Arc.name << " ["
	   << Arc.p1 << ", "
     << Arc.p2 << ", "
     << Arc.id << ", "
	   << Arc.bi_dir << "]";
	return os;
}
