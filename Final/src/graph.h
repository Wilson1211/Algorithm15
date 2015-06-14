#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <queue>

#define DIS_INF 2147483647

using namespace std;

//
class Shape;
class Graph;

extern Graph* graph_; 
class Edge{

	public:
		Edge(Shape* a, Shape* b);
		Shape* shape[2];

		Shape*getNeighbor (Shape* s);

		bool operator < (const Edge& rhs) const;

};
class Window{//new
public:
	float calden();//calculate density
	int area(Shape*);
	int _index;
	int _index1;//y
	int _index2;//x
	int _x0;
	int _x1;
	int _y0;
	int _y1;
	float _density1;
	float _density2;
	float _difference;
	vector<Shape*> member;
};

class Shape{

	public:
		Shape();

		Shape(int index, int x0, int y0, int x1, int y1) : _id(index), _x0(x0), _y0(y0), _x1(x1), _y1(y1) ,repeat(false){};
		void addEdge(Edge *e);
		void sortEdge();
		void inverse();//new
		bool traveled;
		bool repeat;
		int color;
		int _id;
		string name;
		vector<Edge*>edge;
		vector<Window*> window;
		int _x0;
		int _y0;
		int _x1;
		int _y1;
		int _group;//new
		//bool repeat;
};

class Graph{

	public:
		Graph(const string& n);
		~Graph();
		friend class Window;
		void addEdge(const int& v1, const int& v2);
		void sortEdgesOfShape();           
 		void sortShapesByDegree();      
 		void sortShapesByID();
 		void sortWindowsByDensity();               
		void init();		            
		bool readFile( char* filename);
		void DFS(Shape* v, ostream& outfile, int& counter);
		void BFS(Shape* v, ostream& outfile, int& counter, queue<Shape*>& list);
		void Color();
		void connect();
		void reset_travel();
		void optimize();
		Shape * getShapeById(const int& id);

		//print for debug
		void printShapes() ;
		void printWindows();
		void printColor() {};

		//output
		void output(ostream& outfile);
		vector<Window*> windows;//new
		map<int, Shape *> shapesMap;
 		vector<Shape*> shapes;
		vector<Edge*> edges;
		vector<Shape*> groups;//new
		string name;
		int box_x0;
		int box_y0;
		int box_x1;
		int box_y1;
		//parameter
		int alpha;
		int beta;
		int omega;
};

#endif
