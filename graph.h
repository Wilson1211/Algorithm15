#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
<<<<<<< HEAD
#include <queue>
=======
>>>>>>> f52e327d8d8968be75d4fad638cb28ef2244bdad

#define DIS_INF 2147483647

using namespace std;

<<<<<<< HEAD

class Shape;
class Graph;

extern Graph* graph_; 
class Edge{

	public:
		Edge(Shape* a, Shape* b);
		Shape* shape[2];

		Shape*getNeighbor (Shape* s);
=======
class Node;

class Edge{

	public:
		Edge(Node *a, Node *b);
		Node* node[2];

		Node *getNeighbor(Node *n);
>>>>>>> f52e327d8d8968be75d4fad638cb28ef2244bdad

		bool operator < (const Edge& rhs) const;

};

<<<<<<< HEAD
class Shape{

	publicL
		Shape();
		Shape(int index, int x0, int y0, int x1, int y1) : _id(index), _x0(x0), _y0(y0), _x1(x1), _y1(y1) {};
		void addEdge(Edge *e);
		void sortEdge();

		bool traveled;
		int color;
		int _id;
		vetor<Edge*>edge;

		int _x0;
		int _y0;
		int _x1;
		int _y1
=======
class Node{

	public:
		Node(const int& i);
		void addEdge(Edge *e);
		void sortEdge();

		int id;
        int color;
		bool traveled;
		vector<Edge *> edge;

		int d;
		Node *prev;
		int heap_idx;
>>>>>>> f52e327d8d8968be75d4fad638cb28ef2244bdad
};

class Graph{

	public:
		Graph(const string& n);
		~Graph();
		
		void addEdge(const int& v1, const int& v2);
<<<<<<< HEAD
		void sortEdgesOfShape();           //
 		void sortShapesByDegree();      //
 		void sortShapesByID();               //
		void init();		            //
		bool readFile( char* filename);
		void DFS(Shape* v, ostream& outfile, int& counter);
		void BFS(Shape* v, ostream& outfile, int& counter, queue<Shape*>& list);
		void Color(Shape*  v, queue<Shape*>& list);
		void reset_travel();
		Shape * getShapeById(const int& id);

		map<int, Shape *> shapesMap;
 		vector<Shape*> shapes;
		vector<Edge*> edges;
		string name;

		//Final Project
		int alpha;
		int beta;
		int omega;
=======
		void sortEdgesOfNode();
        void sortNodesByDegree();
        void sortNodesByID();
		void init();
		Node * getNodeById(const int& id);

		map<int, Node *> nodesMap;
        vector<Node *> nodes;
		vector<Edge *> edges;
		string name;
>>>>>>> f52e327d8d8968be75d4fad638cb28ef2244bdad
};

#endif
