#include "graph.h"
#include <iostream>
#include <string>
#include <ctype.h>
Graph* graph_ = 0;

Edge::Edge(Shape *a, Shape *b)
{
	if ( a->_id <= b->_id ) { shape[0] = a; shape[1] = b; }
	else { shape[0] = b; shape[1] = a; }
}

bool Edge::operator < (const Edge& rhs) const{

	int id1a = shape[0]->_id;
	int id1b = shape[1]->_id;

	int id2a = rhs.shape[0]->_id;
	int id2b = rhs.shape[1]->_id;

	if ( id1a == id2a ) return id1b < id2b;
	if ( id1a == id2b ) return id1b < id2a;
	if ( id1b == id2a ) return id1a < id2b;
	if ( id1b == id2b ) return id1a < id2a;
    return true;
}
Shape * Edge::getNeighbor(Shape *n)
{
	if ( shape[0] == n ) return shape[1];
	if ( shape[1] == n ) return shape[0];

	return 0;	
}

/*
Shape::Shape(const int& i)
{
	_id = i;
	traveled = false;
	color = 0;
}*/

void Shape::addEdge(Edge *e)
{
	edge.push_back(e);
}

bool edgeComp( const Edge* A, const Edge* B ){
	if ( (*A) < (*B) ) return true;
	return false;
}


void Shape::sortEdge()
{
    sort(edge.begin(), edge.end(), edgeComp);
}

Graph::Graph(const string& n)
{
	name = n;
}

Graph::~Graph()
{
	vector<Edge *>::iterator itE;
	for ( itE = edges.begin() ; itE != edges.end() ; itE++ )
	{
		delete (*itE);
		(*itE) = 0;
	}
	
	map<int, Shape *>::iterator itN;
	for ( itN = shapesMap.begin() ; itN != shapesMap.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}

    vector<Shape *>::iterator itN2;
	for ( itN2 = shapes.begin() ; itN2 != shapes.end() ; itN2++ )
	{
		(*itN2) = 0;
	}
}
/*		
void Graph::addEdge(const int& v1, const int& v2)
{
	Shape *a, *b;
	map<int, Shape *>::iterator it = shapesMap.find(v1);
	if ( it != shapesMap.end() )
		a = (*it).second;
	else
	{
		a = new Shape(v1);
		shapesMap[v1] = a;
        		shapes.push_back(a);
	}

	it = shapesMap.find(v2);
	if ( it != shapesMap.end() )
		b = (*it).second;
	else
	{
		b = new Shape(v2);
		shapesMap[v2] = b;
        		shapes.push_back(b);
	}

	Edge *e = new Edge(a, b);
	edges.push_back(e);

	a->edge.push_back(e);
	b->edge.push_back(e);
}*/

void Graph::addEdge(const int& v1, const int& v2)
{
	Shape *a, *b;
	map<int, Shape*>::iterator it = shapesMap.find(v1);
	if(it != shapesMap.end()) a = (*it).second;
	else cerr << "Shape" << v1 << "doesn't exist. " << endl;

	it = shapesMap.find(v2);
	if(it != shapesMap.end()) a = (*it).second;
	else cerr << "Shape" << v2 << "doesn't exist. " << endl;

	Edge* e = new Edge(a, b);
	edges.push_back(e);

	a->edge.push_back(e);
	b->edge.push_back(e);
}	
	
void Graph::sortEdgesOfShape()
{
	map<int, Shape *>::iterator it;
	for ( it = shapesMap.begin() ; it != shapesMap.end() ; it++ )
	{
		Shape *shape = (*it).second;
		shape->sortEdge();
	}
}

bool ShapeCompByD( const Shape* A, const Shape* B ){
	if ( A->edge.size() > B->edge.size() ) 
		return true;
	else if (A->edge.size() == B->edge.size()) {
		if (A->_id < B->_id) 
			return true;
		else 
 			return false;
    	}
    	else
        		return false;
}

void Graph::sortShapesByDegree()
{
	sort(shapes.begin(), shapes.end(), ShapeCompByD);
}


bool ShapeCompByID( const Shape* A, const Shape* B ){
	if ( A->_id < B->_id ) return true;
	else return false;
}

void Graph::sortShapesByID()
{
	sort(shapes.begin(), shapes.end(), ShapeCompByID);
}
void Graph::init()
{
	map<int, Shape *>::iterator itN;
	for ( itN = shapesMap.begin() ; itN != shapesMap.end() ; itN++ )
	{
		Shape *shape = (*itN).second;
		shape->traveled = false;
		//shape->d = DIS_INF;
		//shape->prev = 0;
 		shape->color = -1;
	}
}

Shape * Graph::getShapeById(const int& id)
{
	return shapesMap[id];
}


bool Graph::readFile( char* filename) {
 	fstream fin(filename);

	if(fin.is_open()) {
		char c; //get single char
		char buf[1024]; //getline
		int parameter[3];
		int shape_count=0;
		
		//set up alpha beta omega
		for(int i=0; i<3; i++) {
			fin.getline(buf,sizeof(buf));
			int pos = 0;
			while(buf[pos] != '=') {
				pos++;
			}
			pos++;
			int num_char=0;
			while(isalnum(buf[pos])) {
				int temp = buf[pos]-'0';
				num_char *= 10;
				num_char += temp;
				pos++;
			}
			parameter[i] = num_char;
		}
		alpha = parameter[0];
		beta = parameter[1];
		omega = parameter[2];

		//set up shapes
		int index=0;
		int coordinate[4]={0,0,0,0};
		while(!fin.eof()) {
			shape_count = 0;
			while(shape_count < 4) {
				fin.get(c);
				int num_char=0;
				while(isalnum(c)) {
					int temp = c - '0';
					num_char *= 10;
					num_char += temp;
					fin.get(c);
				}		
				coordinate[shape_count] = num_char;
				shape_count++;
			}
			// Shape (index, x0, y0, x1, y1)
			Shape* s = new Shape(index, coordinate[0], coordinate[1], coordinate[2], coordinate[3] );
			shapes.push_back(s);
			shapesMap[index++] = s;
		}
		//shapes[0] is dummy shape (0,0) (0,0)
	}
	else {
		return false;
	}
	return true;
}

void Graph::DFS(Shape* v, ostream& outfile, int& counter)
{
	v->traveled = true;
	if(v->edge.size() == 0) {
		//return false;
	}
	else { 
		for(int i=0; i < v->edge.size();i++) {
			if(v->edge[i] != 0) {
				Shape* v_dfs = v->edge[i]->getNeighbor(v);
				if(!v_dfs->traveled) {
					outfile << "v" << v->_id << " -- v" << v_dfs->_id << ";" <<endl;
					counter++;
					DFS(v_dfs, outfile, counter);
				}	
			}
		}
	}	
}

void Graph::BFS(Shape* v, ostream& outfile, int& counter, queue<Shape*>& list)
{
	v->traveled = true;
	if(v->edge.size() == 0) {
		//return false;
	}
	else { 
		for(int i=0; i < v->edge.size();i++) {
			if(v->edge[i] != 0) {
				Shape* v_bfs = v->edge[i]->getNeighbor(v);
				if(!v_bfs->traveled) {
					list.push(v_bfs);
					v_bfs->traveled = true;
					outfile << "v" << v->_id << " -- v" << v_bfs->_id << ";" << endl;
					counter++;
				}	
			}
		}
	}
	if(list.size() != 0) {	
		Shape* next = list.front();
		list.pop();
		BFS(next, outfile, counter, list);
	}
}
/*
void Graph::Color(Shape* v, queue<Shape*>& list)
{
	if(list.size() != 0) {
		Shape* next = list.front();
		list.pop();

		//coloring
		int print = 1;
		vector<int> colorlist;
		for(int i=0; i<v->edge.size(); i++) {
			colorlist.push_back(v->edge[i]->getNeighbor(v)->color);
		}
		sort(colorlist.begin(), colorlist.end());
		for (int i=0;i<colorlist.size();i++) {
			if(print == colorlist[i]) {
				print++;
			}
		}
		v->color = print;

		Color(next, list);
	}
	else {
		int print = 1;
		vector<int> colorlist;
		for(int i=0; i<v->edge.size(); i++) {
			colorlist.push_back(v->edge[i]->getNeighbor(v)->color);
		}
		sort(colorlist.begin(), colorlist.end());
		for (int i=0;i<colorlist.size();i++) {
			if(print == colorlist[i]) {
				print++;
			}
		}
	}
}
*/

void Graph::reset_travel()
{
	map<int, Shape *>::iterator it;
	for ( it = shapesMap.begin() ; it != shapesMap.end() ; it++ )
	{
		Shape *shape = (*it).second;
		shape->traveled = false;
	}
}