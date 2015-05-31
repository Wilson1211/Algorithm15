//#include "graph.h"
#include <map>
#include <cmath>

//////////////////////
#include <iostream>
#include <vector>
using namespace std;
int alpha = 10 , beta = 10;
class Shape{
	public:
		Shape();
		Shape(int index, int x0, int y0, int x1, int y1) : _id(index), _x0(x0), _y0(y0), _x1(x1), _y1(y1) {};
		//void addEdge(Edge *e);
		//void sortEdge();
/*
		bool traveled;
		int color;*/
		int _id;
		string name;/*
		vector<Edge*>edge;
*/
		int _x0;
		int _y0;
		int _x1;
		int _y1;
};
vector<Shape*> shapes;

///////////////////////

struct cmpAbsInt {
    bool operator()(const int a, const int b) {//if error, send ref
        return abs(a) < abs(b);
    }
};

void connect() {
	map<int, Shape*, cmpAbsInt> x, y;
	for(int i = 0; i < shapes.size(); ++i) {
		x[ shapes[i]->_x0 ] = shapes[i];
		x[-shapes[i]->_x1 ] = shapes[i];
		y[ shapes[i]->_y0 ] = shapes[i];//Suppose that upper-right point is negative
		y[-shapes[i]->_y1 ] = shapes[i];
	}

	for(map<int, Shape*>::iterator i = x.begin(), j; i != x.end(); ++i) {
		if(i->first < 0) {
			for(j = i; j != x.end() && abs(j->first) < abs(i->first) + alpha; ++j)
				if(j->first > 0) {
					if(i->second->_y0 < j->second->_y0 && j->second->_y0 < i->second->_y1)
						addEdge(i->second->_id, j->second->_id);
					else if(i->second->_y0 < j->second->_y1 && j->second->_y1 < i->second->_y1)
						addEdge(i->second->_id, j->second->_id);
					else if(j->second->_y0 < i->second->_y0 && i->second->_y0 < j->second->_y1)
						addEdge(i->second->_id, j->second->_id);
					else if(j->second->_y0 < i->second->_y1 && i->second->_y1 < j->second->_y1)
						addEdge(i->second->_id, j->second->_id);
				}
		}
	}

	for(map<int, Shape*>::iterator i = y.begin(), j; i != y.end(); ++i) {
		if(i->first < 0)
			for(j = i; j != y.end() && abs(j->first) < abs(i->first) + beta; ++j)
				if(j->first > 0) {
					if(i->second->_x0 < j->second->_x0 && j->second->_x0 < i->second->_x1)
						addEdge(i->second->_id, j->second->_id);
					else if(i->second->_x0 < j->second->_x1 && j->second->_x1 < i->second->_x1)
						addEdge(i->second->_id, j->second->_id);
					else if(j->second->_x0 < i->second->_x0 && i->second->_x0 < j->second->_x1)
						addEdge(i->second->_id, j->second->_id);
					else if(j->second->_x0 < i->second->_x1 && i->second->_x1 < j->second->_x1)
						addEdge(i->second->_id, j->second->_id);
				}
	}
}

int main() {
	Shape *a = new Shape(1, 1, 1, 8, 8);
	Shape *b = new Shape(2, 11, 1, 12, 20);
	Shape *c = new Shape(3, 1, 11, 7, 20);

	shapes.push_back(a);
	shapes.push_back(b);
	shapes.push_back(c);

	connect();
	return 0;
}