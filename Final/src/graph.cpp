#include "graph.h"
#include <iostream>
#include <string>
#include <ctype.h>
Graph* graph_ = 0;

size_t
myStrGetTok(const string& str, string& tok, size_t pos = 0,
            const char del = ' ')
{
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin; }
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}

Edge::Edge(Shape *a, Shape *b)
{//cout<<"Egg1";
	if ( a->_id <= b->_id ) { shape[0] = a; shape[1] = b;}
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
	cout << "(" << v1 << ", " << v2 <<")\n";
	Shape *a, *b;
	map<int, Shape*>::iterator it = shapesMap.find(v1);
	if(it != shapesMap.end() /*&& (it != NULL)*/) {a = (*it).second;}
	else cerr << "Shape" << v1 << "doesn't exist. " << endl;

	it = shapesMap.find(v2);
	if(it != shapesMap.end()/*&& (it != NULL)*/) {b = (*it).second;}
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
			Shape* s = new Shape(++index, coordinate[0], coordinate[1], coordinate[2], coordinate[3] );
			shapes.push_back(s);
			shapesMap[index] = s;
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
void encolor(Shape* u){
        vector<Edge* >::iterator it;
        Shape* node;
        int flag = 1;
        it = u->edge.begin();
        while(it!= u->edge.end()){

            node = (*it)->getNeighbor(u);
            if(flag == node->color){
                flag++;
                it = u->edge.begin();
            }else{
                it++;
            }
        }
        u->color = flag;
    }
    void Colorvisit(Shape* u){
        vector<Edge*>::iterator it;
        it = (u->edge).begin();
        Shape* node;
        while(it != (u->edge).end()){
            node = (*it)->getNeighbor(u);
            if(node->edge.size() == u->edge.size()){
                if(node->color == 0){
                    encolor(node);
                    Colorvisit(node);
                }
            }
            it++;
        }
        return;
    }
void Graph::Color()
{
	    graph_->sortShapesByDegree();
        //CommonNs::TmUsage tmusg;
        //CommonNs::TmStat stat;
        vector<Shape *>::iterator it1;
        vector<Edge *>::iterator it2;
        int flag = 1;
        int max_color = 1;
        Shape * node ;

        it1 = (graph_->shapes).begin();
        while(it1 != graph_->shapes.end()){
            (*it1)->color = 0;
            it1++;
        }

        it1 = (graph_->shapes).begin();
        int edgemax = (*it1)->edge.size();
        //tmusg.periodStart();
        while(it1!=(graph_->shapes).end()){//first check all max edges degree vertices, and color them
            //if((*it1)->edge.size() < edgemax){break;}
            if((*it1)->color != 0){it1++;continue;}
            encolor(*it1);
            Colorvisit(*it1);
            it1++;
        }
}


void Graph::printShapes()
{
	map<int, Shape *>::iterator it;
	
	cout << alpha << ", " << beta << ", " << omega << endl;
	for ( it = shapesMap.begin() ; it != shapesMap.end() ; it++ )
	{
		Shape *shape = (*it).second;
		cout << "#shape<"<< shape->_id << ">" << endl;
		cout << shape->_x0 << ", " << shape->_y0 << ", " << shape->_x1 << ", " << shape->_y1 << endl;
		cout << "	edges: " << endl;
		for(int i=0; i<shape->edge.size();i++) {
			cout << "	-shape" << shape->edge[i]->getNeighbor(shape)->_id << endl;
		}
		cout << "	color:" << shape->color << endl;
	}
}

void Graph::reset_travel()
{
	map<int, Shape *>::iterator it;
	for ( it = shapesMap.begin() ; it != shapesMap.end() ; it++ )
	{
		Shape *shape = (*it).second;
		shape->traveled = false;
	}
}


#include <map>
#include <cmath>
struct cmpAbsInt {
    bool operator()(const int a, const int b) {//if error, send ref
        return abs(a) < abs(b);
    }
};
void Graph::connect() {
	multimap<int, Shape*, cmpAbsInt> x, y;
	for(int i = 0; i < shapes.size(); ++i) {
		x.insert( pair<int, Shape*>( shapes[i]->_x0, shapes[i]) );
		x.insert( pair<int, Shape*>(-shapes[i]->_x1, shapes[i]) );
		y.insert( pair<int, Shape*>( shapes[i]->_y0, shapes[i]) );
		y.insert( pair<int, Shape*>(-shapes[i]->_y1, shapes[i]) );
		/*
		x[ shapes[i]->_x0 ] = shapes[i];
		x[-shapes[i]->_x1 ] = shapes[i];
		y[ shapes[i]->_y0 ] = shapes[i];//Suppose that upper-right point is negative
		y[-shapes[i]->_y1 ] = shapes[i];*/
	}

	for(multimap<int, Shape*>::iterator i = x.begin(), j; i != x.end(); ++i) {
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
	for(multimap<int, Shape*>::iterator i = y.begin(), j; i != y.end(); ++i) {
		if(i->first < 0) {
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
}

void Graph::output(ostream& outfile)
{/*
	//output windows
	for(int i=0; i< windows.size(); i++) {
		outfile << "WIN[" << i << "]=" << windows[i]->_x0 << "," << windows[i] << ->_y0 << "," 
					       << windows[i]->_x1 << "," << windows[i] << ->_y1 
					       << "(" << windows[i]->density0 << " " << windows[i]->density1 << ")" << endl;
	}

	//output group
	for(int i=0; i<groups.size(); i++) {
		vector<Shape*> color1;
		vector<Shape*> color2;
		vector<Shape*> color0; // NO COLOR

		for()

		output << "GROUP" << endl;
		otuptu << 
	}*/
}