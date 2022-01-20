#include "Graph.h"

Graph::Graph() = default;

//function recieves a pointer to a point on graph, returns a vector of pointers to arcs that lead away from that point
std::vector<Arc *> Graph::arcsFrom(Point * p){
	std::vector<Arc *> result;
	for(int i = 0; i < this->arcs.size(); i++){
		if(this->arcs[i].p1->id == p->id || (this->arcs[i].p2->id == p->id && this->arcs[i].bi_dir)){
			result.push_back(&this->arcs[i]);
		}
	}

	return result;
}

//function recieves a pointer to a point on graph, returns a vector of pointers to arcs that lead towards that point
std::vector<Arc *> Graph::arcsTo(Point * p){
	std::vector<Arc *> result;
	for(int i = 0; i < this->arcs.size(); i++){
		if(this->arcs[i].p2->id == p->id || (this->arcs[i].p1->id == p->id && this->arcs[i].bi_dir)){
			result.push_back(&this->arcs[i]);
		}
	}
	return result;
}

std::vector<Arc *> Graph::djikstra(Point *a, Point *b){
	std::vector<Arc *> result;
	//initialise start point
	a->visited = true;
	a->dist = 0;
	Point *currentPoint = a;
	while(true){
		if(b->visited){
			break;
		}
		//increment distances of all points connected to starting point if smaller than current dist
		std::vector<Arc *> arcsFromCurrent = arcsFrom(currentPoint);
		for(int i = 0; i < arcsFromCurrent.size(); i++){
			Arc *c = arcsFromCurrent[i];
			if(c->p1->id == currentPoint->id){
				c->p2->dist = std::min(currentPoint->dist + 1, c->p2->dist);
			} else if(c->p2->id == currentPoint->id){
				c->p1->dist = std::min(currentPoint->dist + 1, c->p1->dist);
			}
		}
		//find unvisited point with lowest dist
		Point *lowest = NULL;
		for(int i = 0; i < this->points.size(); i++){
			if(!this->points[i].visited){
				if(lowest == NULL){
					lowest = &this->points[i];
				} else if(this->points[i].dist < lowest->dist) {
					lowest = &this->points[i];
				}
			}
		}
		//if all points visited, leave, otherwise set lowest unvisited point to currentpoint and visited to true
		if(lowest == NULL){
			break;
		} else {
			lowest->visited = true;
			currentPoint = lowest;
		}
	}

	//backtrack to find route
	currentPoint = b;
	while(currentPoint->id != a->id){
		std::vector<Arc *> arcsToCurrent = arcsTo(currentPoint);
		for(int i = 0; i < arcsToCurrent.size(); i++){
			Arc *c = arcsToCurrent[i];
			if(c->p1->id == currentPoint->id && c->p2->dist == currentPoint->dist-1){
				c->inRoute = true;
				result.push_back(c);
				currentPoint = c->p2;
				break;
			} else if(c->p2->id == currentPoint->id && c->p1->dist == currentPoint->dist-1){
				c->inRoute = true;
				result.push_back(c);
				currentPoint = c->p1;
				break;
			}
		}
	}
	std::reverse(result.begin(),result.end());
	return result;
}
