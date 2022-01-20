#include "Arc.h"
#include "Point.h"
#include <math.h>

Arc::Arc() = default;
Arc::Arc(Point *initP1, Point *initP2, int initID, bool initDir) : p1(initP1), p2(initP2), id(initID), bi_dir(initDir) {}

float Arc::length(int routeMode){
	if(routeMode == 0){
		return 1.0f;
	} else if(routeMode == 1){
		int dx = p1->x - p2->x;
		int dy = p1->y - p2->y;
		return sqrt(dx*dx + dy*dy);
	} else if(routeMode == 2){
		return preloaded_length;
	}
	return 1.0f;
}

std::ostream &operator<<(std::ostream &os, const Arc &Arc) {
	os << Arc.name << " ["
	   << Arc.p1 << ", "
     << Arc.p2 << ", "
     << Arc.id << ", "
	   << Arc.bi_dir << "]";
	return os;
}
