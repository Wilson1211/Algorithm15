#include "graph.h"
#include <iostream>
#include <string>
#include <ctype.h>
#include <queue>
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

#include <stdio.h>
bool Graph::readFile( char* filename) {
	/////C style //////
	/*FILE *fin = fopen(filename, "r");
	if(fin != NULL) {
		int a,b,c,d,index=0;
		Shape* s;
		fscanf(fin,"ALPHA=%d\n", &alpha);
		fscanf(fin,"BETA=%d\n" , &beta);
		fscanf(fin,"OMEGA=%d\n", &omega);
		while(fscanf(fin,"%d,%d,%d,%d\n", &a, &b, &c, &d) != EOF) {
			s = new Shape(++index, a, b, c, d);
			shapes.push_back(s);
			shapesMap[index] = s;
		}
		fclose(fin);
		return true;
	}
	else	return false;*/
	//////C++ style///////
	cout<<"!!!!"<<endl;
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
				int sign=1;
				/*if(c == '-')
					sign = -1;*/ 
				int num_char=0;
				while(isalnum(c) || c=='-') {
					if(c=='-') {
						sign = -1;
						fin.get(c);
					}
					else {
						int temp = c - '0';
						num_char *= 10;
						num_char += temp;
						fin.get(c);
					}
				}		
				num_char *= sign;
				coordinate[shape_count] = num_char;
				shape_count++;
			}
			// Shape (index, x0, y0, x1, y1)
			Shape* s = new Shape(++index, coordinate[0], coordinate[1], coordinate[2], coordinate[3] );
			shapes.push_back(s);
			shapesMap[index] = s;
		}
		//shapes[0] is dummy shape (0,0) (0,0)
		shapes.pop_back();
		shapesMap.erase(index);
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
int encolor(Shape* u){
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
        if(flag >2){u->color = 3;return 0;}
        else{return 1;}
}
int i=1;
int global_int = -1;
bool Colorvisit(Shape* u){
		i = 1;
		int k1 = 1;
        vector<Edge*>::iterator it;
        it = (u->edge).begin();
        Shape* node;
        while(it != (u->edge).end()){
            node = (*it)->getNeighbor(u);
            if(node->edge.size() == u->edge.size()){
                if(node->color == 0){
                    i = encolor(node);
                    if(i == 0){u->color = 3;return 0;}
                    k1 = Colorvisit(node);
                    if(k1 == 0){return 0;}
                }else if(node->color == 3){
                	u->color = 3;
                	i = 0;
                	return 0;
                }
            }
            if(i == 0){u->color = 3;break;}
            it++;
        }
        if(i==0){return 0;}
        else { return 1;}
}
void decolor(Shape* u){
	Shape* node;
	u->color = 3;
	vector<Edge*>::iterator it = (u->edge).begin();
	while(it != (u->edge).end()){
		node = (*it)->getNeighbor(u);
		if(node->color != 3){decolor(node);}
		it++;
	}

	return;
}

vector<Shape*> flipv;
void flipdfs(Shape* u){
	if(u->traveled == 1){return;}
	Shape* node;
	vector<Edge*>::iterator it = (u->edge).begin();

	if(u->color == 1){u->color = 2;}
	else if(u->color == 2){u->color = 1;}
	u->traveled = 1;
	flipv.push_back(u);
	while(it!= (u->edge).end()){
		node = (*it)->getNeighbor(u);
		flipdfs(node);
		it++;
	}

}

void flipcolor(Shape* u){
	graph_->reset_travel();
	while(flipv.size()!= 0){ flipv.pop_back(); }

	Shape* node;
	vector<Edge*>::iterator it = (u->edge).begin();
	if(u->color == 1){u->color = 2;}
	else if(u->color == 2){u->color = 1;}
	u->traveled = 1;
	flipv.push_back(u);
	while(it!= (u->edge).end()){
		node = (*it)->getNeighbor(u);
		if(node->traveled == 0){
			flipdfs(node);
		}

		it++;
	}
}

void flipbackcolor(){
	vector<Shape*>::iterator it = flipv.begin();
	while(it != flipv.end()){
		if((*it)->color == 1){
			(*it)->color = 2;
		}else if((*it)->color == 2){(*it)->color = 1;}
		it++;
	}
}

void dfs_visit(Shape* u){
	if(u->_group != -1){return ;}
	vector<Edge*>::iterator it = (u->edge).begin();
	Shape* node;
	while(it!= (u->edge).end()){cout<<"778787"<<endl;
		//if(u->_group == -1){
			u->_group = global_int;
			node = (*it)->getNeighbor(u);
			if(node->_group == -1){dfs_visit(node);}
			
		//}
		it++;
	}

}

//int global_int = -1;
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
        int k1;
        //tmusg.periodStart();
        while(it1!=(graph_->shapes).end()){//first check all max edges degree vertices, and color them
            //if((*it1)->edge.size() < edgemax){break;}
            if((*it1)->color != 0){it1++;continue;}
            
            encolor(*it1);
            k1 = Colorvisit(*it1);
            if(k1 == 0){decolor(*it1);}
            it1++;
        }
        it1 = (graph_->shapes).begin();
        /////////////
        graph_->sortShapesByID();
        while(it1!=(graph_->shapes).end()){//first check all max edges degree vertices, and color them
           	if((*it1)->color == 3){(*it1)->color = 0;}
           	it1++;
        }
        ////////////////// calculate windows density
        
        int x1, x2, x3, x4;
        int y1, y2, y3, y4;
        int i, j, flag1 = 0, flag2 = 0;
        //box_x0 = 0;box_x1 = 0;box_y0 = 0;box_y1= 0;
       vector<Shape*>::iterator it = (graph_->shapes).begin();
       
        while(it != (graph_->shapes).end()){

        	if((*it)->color != 0){
        		if(flag1 == 0){
        			box_x0 = (*it)->_x0;
        			box_x1 = (*it)->_x1;
        			box_y0 = (*it)->_y0;
        			box_y1 = (*it)->_y1;
        			flag1 = 1;
        		}
        		/*
        		cout<<(*it)->_id<<endl;
        		cout<<(*it)->_x0 <<endl;
        		cout<<(*it)->_y0 <<endl;*/
        		box_x0 = (box_x0>(*it)->_x0)? (*it)->_x0: box_x0;
        		box_x1 = (box_x1<(*it)->_x1)? (*it)->_x1: box_x1;
        		box_y0 = (box_y0>(*it)->_y0)? (*it)->_y0: box_y0;
        		box_y1 = (box_y1<(*it)->_y1)? (*it)->_y1: box_y1;
        	}
    
        	it++;
        }
    	//cout<<"boxx0 "<<box_x0<<" boxx1 "<<box_x1<<" boxy0 "<<box_y0<<" box_y1 "<<box_y1<<endl;
        i = (box_x1 - box_x0)/omega;//how many windows in x in the box
        if( (box_x1 - box_x0) % omega != 0){flag1 = 1;i++;}
        j = (box_y1 - box_y0)/omega;//how many windows in y in the box
        if((box_y1 - box_y0) % omega != 0){flag2 = 1;j++;}
        
        
        it = (graph_->shapes).begin();
        int box_index1 ,box_index2;
        
        int k, l;
        for(k=0;k<i;k++){
        	for(l=0;l<j;l++){
        		Window* w = new Window;
        		w->_index2 = l;
        		w->_index1 = k;
        		w->_index = l+k;
        		if(flag1 == 1&&(l == j-1)){
        			w->_x1 = box_x1;
        			w->_x0 = w->_x1 - omega;
        		}else{
        			w->_x0 = box_x0 + l*omega;
        			w->_x1 = w->_x0 + omega;
        		}
        		if(flag2 == 1 && (k == i-1)){
        			w->_y1 = box_y1;
        			w->_y0 = w->_y1 - omega;
        		}else{
        			w->_y0 = box_y0 + k * omega;
        			w->_y1 = w->_y0 + omega;
        		}

        		(graph_->windows).push_back(w);
        		//cout<<graph_->windows.size()<<endl;
        	}
        	
        }
        Window* w;
        /*for(k=0;k<i;k++){
        	for(l=0;l<j;l++){
        		w = graph_->windows[l+k*i];
        		cout<<"w_idx "<<w->_index<<endl;
        		cout<<"w_idx1 "<<w->_index1<<endl;
				cout<<"w_idx2 "<<w->_index2<<endl;
				cout<<"w_x0 "<<w->_x0<<endl;
				cout<<"w_y0 "<<w->_y0<<endl;
				cout<<"w_x1 "<<w->_x1<<endl;
				cout<<"x_y1 "<<w->_y1<<endl;
				cout<<endl;
				cout<<endl;
        	}
        	
        }*/
        //Window* w;
        	
        it = (graph_->shapes).begin();
        while(it!=(graph_->shapes).end()){
        	if((*it)->color != 0){//cout<<"11"<<endl;
	        	x1 = ((*it)->_x0 - box_x0)/omega;//
	        	y1 = ((*it)->_y0 - box_y0)/omega;//
				//box_index1 = x1 + y1 * i;
	        	x2 = ((*it)->_x1 - box_x0 -0.1)/omega;//
	        	//if((*it)->_x1 == box_x1){x2--;}
	        	y2 = ((*it)->_y1 - box_y0 -0.1)/omega;//
	        	//if((*it)->_y1 == box_y1){y2--;}
				//box_index2 = x2 + y2*i;
	        	/*cout<<y1<<endl;
	        	cout<<y2<<endl;
	        	cout<<x1<<endl;
	        	cout<<x2<<endl;
	        	cout<<i<<endl;
	        	cout<<graph_->windows.size()<<endl;*/
				for(int b = y1; b <= y2; ++b){
					for(int a = x1; a <= x2; ++a){
						cout << a+b*i<<"!!!\n";
						w = graph_->windows[a + b*i];
						(*it)->window.push_back(w);
						(w->member).push_back(*it);
					}
				}//cout<<"112"<<endl;
				//rightmost up-most
				//x1 = x3, x2 = x4, y1 = y3, y2= y4
				x3 = (box_x1 - (*it)->_x0)/omega;
				y3 = (box_y1 - (*it)->_y0)/omega;
				x4 = (box_x1 - (*it)->_x1)/omega;
				y4 = (box_y1 - (*it)->_y1)/omega;


				/*cout<<"x3 "<<x3<<endl;
				cout<<"y3 "<<y3<<endl;
				cout<<"x4 "<<x4<<endl;
				cout<<"y4 "<<y4<<endl;
				cout<<endl;*/
				if(omega * j + box_y0 != box_y1){
					if((y2 != j-1) && (y4 == 0)){
						for(int a = x4; a <= j-1-x2; ++a) {//cout<<"id "<<(*it)->_id<<endl;cout<<"asdfasdf"<<endl;
							w = graph_->windows[ i*j-1 - a];
							(*it)->window.push_back(w);
							(w->member).push_back(*it);
						}
						/*if((x2!=j-1) && (x4 == 0)){
							w = graph_->windows[i*j-1];
							(*it)->window.push_back(w);
							(w->member).push_back(*it);
						}*/
					}
				}
				//cout<<"89898989"<<endl;
				if(omega * i + box_x0 != box_x1){
					if((x2!=j-1) && (x4 == 0)){
						for(int b = y4; b <= j-1-y2; ++b) {//cout<<"id "<<(*it)->_id<<endl;cout<<"jeeieieie"<<endl;
							//if((*it)->_id == 15){cout<<x1<<" "<<x2<<" "<<x3}
							if((y4 == 0)&&(y2 != j-1)){
								if(b != y4){
								w = graph_->windows[ i*j-1 - b*i];
								(*it)->window.push_back(w);
								(w->member).push_back(*it);
								}
							}else{
								w = graph_->windows[ i*j-1 - b*i];
								(*it)->window.push_back(w);
								(w->member).push_back(*it);
							}
						}
					}
				}
			}
        	it++;
        }
		vector<Window*>::iterator itw = windows.begin();
		while(itw != windows.end()){
			(*itw)->calden();
			itw++;
		}

		///////////////////input group
		it = (graph_->shapes).begin();//int k, l
		k = 0;
		while(it != (graph_->shapes).end()){
			if((*it)->_group == -1){global_int++; dfs_visit((*it));}

			it++;
		}
		/*
		vector<Shape*> group_out[global_int+1]; 
		while(it!= (graph_->shapes).end()){
			group_out[(*it)->_group].push_back((*it));
			it++;
		}
		while(k != global_int+1){
			group.push_back();
			k++;
		}*/

}


void Graph::printShapes()
{
	map<int, Shape *>::iterator it;



	/*vector<Window*>::iterator itt1;
	cout<<"sdfsdfsssssssssssssssssssss"<<endl;
	vector<Shape*>::iterator itt; 
	vector<Shape*>::iterator itt2;
	itt1 = windows.begin();
	itt1++;
	itt = (*itt1)->member.begin();
	itt++;
	flipcolor(*itt);
	itt2 = flipv.begin();
	while(itt2!= flipv.end()){
		cout<<(*itt2)->_id<<"      "<<(*itt2)->color<<endl;
		itt2++;
	}
	cout<<"sdfsdfsssssssssssssssssssss"<<endl;
	flipbackcolor();
	itt2 = flipv.begin();
	while(itt2!= flipv.end()){
		cout<<(*itt2)->_id<<"      "<<(*itt2)->color<<endl;
		itt2++;
	}
	cout<<"sdfsdfsssssssssssssssssssss"<<endl;*/



	cout << alpha << ", " << beta << ", " << omega << endl;
	for ( it = shapesMap.begin() ; it != shapesMap.end() ; it++ )
	{
		Shape *shape = (*it).second;
		cout << "#shape<"<< shape->_id << ">" << endl;
		cout<< "which group "<<shape->_group<<endl; 
		cout << shape->_x0 << ", " << shape->_y0 << ", " << shape->_x1 << ", " << shape->_y1 << endl;
		cout << "	edges: " << endl;
		for(int i=0; i<shape->edge.size();i++) {
			cout << "	-shape" << shape->edge[i]->getNeighbor(shape)->_id << endl;
		}
		cout << "	color:" << shape->color << endl;
	}

	vector<Window*>::iterator it1;
	
	cout << "Windows: " << windows.size() << endl;
	cout << "Window length" << omega << endl;
	for ( it1 = windows.begin() ; it1 != windows.end() ; it1++ )
	{
		cout << "#window" << (*it1)->_index1 << (*it1)->_index2 << ":"<< endl;
		cout << "	density:" << (*it1)->_density1 << ", " << (*it1)->_density2 << endl;
		cout << "	size:" << (*it1)->member.size() << endl;
		cout << "	" ;
		for(int i=0; i<(*it1)->member.size(); i++) 
			cout << (*it1)->member[i]->_id << ", " ;
		cout << endl;
	}



}

void Graph::printWindows()
{
	vector<Window*>::iterator it;


	cout << "Windows: " << windows.size() << endl;
	cout << "Window length" << omega << endl;
	for ( it = windows.begin() ; it != windows.end() ; it++ )
	{
		cout << "#window" << (*it)->_index1 << (*it)->_index2 << ":"<< endl;
		cout << "	density:" << (*it)->_density1 << ", " << (*it)->_density2 << endl;
		cout << "	size:" << (*it)->member.size() << endl;
		cout << "	" ;
		for(int i=0; i<(*it)->member.size(); i++) 
			cout << (*it)->member[i]->_id << ", " ;
		cout << endl;
	}
	for (int i=0; i<shapes.size(); i++) 
		cout << shapes[i]->_id << ", " << endl;

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
/*struct cmpAbsInt {
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
	}
	multimap<int, Shape*>::iterator i, j;
	for(i = x.begin(); i != x.end(); ++i) {
		if(i->first < 0)
			for(j = i; j != x.end() && abs(j->first) < alpha - i->first; ++j)
				if(j->first > 0) {
					if( (i->second->_y0 < j->second->_y0 && j->second->_y0 < i->second->_y1)
					  ||(i->second->_y0 < j->second->_y1 && j->second->_y1 < i->second->_y1)
					  ||(j->second->_y0 < i->second->_y0 && i->second->_y0 < j->second->_y1)
					  ||(j->second->_y0 < i->second->_y1 && i->second->_y1 < j->second->_y1) )
						addEdge(i->second->_id, j->second->_id);
				}
	}
	for(i = y.begin(); i != y.end(); ++i) {
		if(i->first < 0)
			for(j = i; j != y.end() && abs(j->first) < beta - i->first; ++j)
				if(j->first > 0) {
					if( (i->second->_x0 < j->second->_x0 && j->second->_x0 < i->second->_x1)
					  ||(i->second->_x0 < j->second->_x1 && j->second->_x1 < i->second->_x1)
					  ||(j->second->_x0 < i->second->_x0 && i->second->_x0 < j->second->_x1)
					  ||(j->second->_x0 < i->second->_x1 && i->second->_x1 < j->second->_x1) )
						addEdge(i->second->_id, j->second->_id);
				}
	}
}
*/
class LR{
public:
	LR();
	LR(int v, bool r): value(v), right(r){};
	int value;
	bool right;
};
struct cmpAbsInt {
    bool operator()(const LR& a, const LR& b) {//if error, send ref
        return a.value < b.value;
    }
};
void Graph::connect() {
	multimap<LR, Shape*, cmpAbsInt> x, y;
	for(int i = 0; i < shapes.size(); ++i) {
		x.insert( pair<LR, Shape*>( LR(shapes[i]->_x0, false), shapes[i]) );
		x.insert( pair<LR, Shape*>( LR(shapes[i]->_x1, true ), shapes[i]) );
		y.insert( pair<LR, Shape*>( LR(shapes[i]->_y0, false), shapes[i]) );
		y.insert( pair<LR, Shape*>( LR(shapes[i]->_y1, true ), shapes[i]) );
	}
	multimap<LR, Shape*>::iterator i, j;
	for(i = x.begin(); i != x.end(); ++i) {
		if((i->first).right)
			for(j = i; j != x.end() && alpha + (i->first).value > (j->first).value; ++j)
				if((j->first).right==false) {
					if( (i->second->_y0 < j->second->_y0 && j->second->_y0 < i->second->_y1)
					  ||(i->second->_y0 < j->second->_y1 && j->second->_y1 < i->second->_y1)
					  ||(j->second->_y0 < i->second->_y0 && i->second->_y0 < j->second->_y1)
					  ||(j->second->_y0 < i->second->_y1 && i->second->_y1 < j->second->_y1) )
						addEdge(i->second->_id, j->second->_id);
				}
	}
	for(i = y.begin(); i != y.end(); ++i) {
		if((i->first).right)
			for(j = i; j != y.end() && beta  + (i->first).value > (j->first).value; ++j)
				if((j->first).right==false) {
					if( (i->second->_x0 < j->second->_x0 && j->second->_x0 < i->second->_x1)
					  ||(i->second->_x0 < j->second->_x1 && j->second->_x1 < i->second->_x1)
					  ||(j->second->_x0 < i->second->_x0 && i->second->_x0 < j->second->_x1)
					  ||(j->second->_x0 < i->second->_x1 && i->second->_x1 < j->second->_x1) )
						addEdge(i->second->_id, j->second->_id);
				}
	}
}

bool WindowCompByDensity( const Window* A, const Window* B ){
	if(A->_difference > B-> _difference) 
		return true;
	else 
		return false;
}


void Graph::sortWindowsByDensity()
{
	sort(windows.begin(), windows.end(), WindowCompByDensity);
}
/*vector<Shape*> flipv;
void flipdfs(Shape* u){
	if(u->traveled == 1){return;}
	Shape* node;
	vector<Edge*>::iterator it = (u->edge).begin();

	if(u->color == 1){u->color = 2;}
	else if(u->color == 2){u->color = 1;}
	u->traveled = 1;
	flipv.push_back(u);
	while(it!= (u->edge).end()){
		node = (*it)->getNeighbor(u);
		flipdfs(node);
		it++;
	}

}

void flipcolor(Shape* u){
	graph_->reset_travel();
	while(flipv.size()!= 0){ flipv.pop_back(); }

	Shape* node;
	vector<Edge*>::iterator it = (u->edge).begin();
	if(u->color == 1){u->color = 2;}
	else if(u->color == 2){u->color = 1;}
	u->traveled = 1;
	flipv.push_back(u);
	while(it!= (u->edge).end()){
		node = (*it)->getNeighbor(u);
		if(node->traveled == 0){
			flipdfs(node);
		}

		it++;
	}
}

void flipbackcolor(){
	vector<Shape*>::iterator it = flipv.begin();
	while(it != flipv.end()){
		if((*it)->color == 1){
			(*it)->color = 2;
		}else if((*it)->color == 2){(*it)->color = 1;}
		it++;
	}
}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Graph::optimize()
{
	graph_->sortWindowsByDensity();

	queue<Shape*> tasklist, tasklist_cross;

	vector<Window*>::iterator it;
	float	form_diff;
	for(it = windows.begin(); it != windows.end(); it++) {
		//int group; use group can avoid duplicated shapes
		for(int i=0; i<(*it)->member.size(); i++) {
			if((*it)->member[i]->window.size() > 1 )
				tasklist_cross.push((*it)->member[i]);
			else
				tasklist.push((*it)->member[i]);
		}
	}

	while(tasklist.size() != 0) {
		Shape* shape = tasklist.front();
		tasklist.pop();
		form_diff = shape->window[0]->_difference;
		if(!(shape->repeat)) {
			flipcolor(shape);
			//shape->window[0]->calden();
			if(form_diff < shape->window[0]->calden())
				flipbackcolor();
		}	
	}

	while(tasklist_cross.size()){
		Shape* shape = tasklist_cross.front();
		tasklist_cross.pop();
		form_diff = shape->window[0]->_difference;
		if(!(shape->repeat)) {
			flipcolor(shape);
			//shape->window[0]->calden();
			if(form_diff < shape->window[0]->calden())
				flipbackcolor();
		}	
	}
	

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Graph::output(ostream& outfile)
{/*
	////////////////////////////////





	///////////////////////////////


	//output windows
	for(int i=0; i< windows.size(); i++) {
		outfile << "WIN[" << i << "]=" << windows[i]->_x0 << "," << windows[i] << ->_y0 << "," 
					       << windows[i]->_x1 << "," << windows[i] << ->_y1 
					       << "(" << windows[i]->density0 << " " << windows[i]->density1 << ")" << endl;
	}


	vector<Shape*> color1;
	vector<Shape*> color2;
	vector<Shape*> color0; // NO COLOR
	//output group
	for(int i=0; i<groups.size(); i++) {

		//////////////////////////////////////// what is the structure of "groups" ??????????????????????????
		//
		//vecotr<Group*> groups ????
		//class Group {
		//	private: vector<Shape*> member
		//}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Shape* shape = groups[i]->member[j];
		for(int j=0;groups[i]->size();j++) {
			if(shape->color == 1) {
				color1.push_back(shape);
			}
			else if (shape->color == 2) {
				color2.push_back(shape);
			}
			else {
				color0.push_back(shape);
			}
		}
		output << "GROUP" << endl;

		//NO color
		for(int no=1; no <=color0.size();no++) {
			outfile << "NO[" << no << "]=" << color0[no-1]->_x0<<"," << color0[no-1]->_y0 << "," 
						         << color0[no-1]->_x1 << "," << color0[no-1]->_x1 << endl;
		}
		for(int i=1; i <=color1.size();i++) {
			outfile << "CA[" << i << "]=" << color1[i-1]->_x0<<"," << color1[i-1]->_y0 << "," 
						    << color1[i-1]->_x1 << "," << color1[i-1]->_x1 << endl;
		}
		for(int i=1; i <=color2.size();i++) {
			outfile << "CB[" << i << "]=" << color2[i-1]->_x0<<"," << color2[i-1]->_y0 << "," 
						    << color2[i-1]->_x1 << "," << color2[i-1]->_x1 << endl;
		}
	}
	///////////////////////////////
	//GROUP
            	//NO[1]=0,200,185,260
            	//NO[2]=180,50,400,150
            	//NO[3]=100,330,280,400
            	//NO[4]=320,290,480,340
            	//NO[5]=310,395,460,450
	//
            	//GROUP
            	//CA[1]=720,120,940,220
            	//CA[2]=640,400,820,470
            	//CB[1]=540,270,725,330
            	//CB[2]=860,360,1020,410
	////////////////////////////////
*/

}


//int Window::area(Shape* a){

int Window::area(Shape* a){

	/*int x1, x0, y1, y0;//indicates window coordinates
	int omega = graph_->omega;
	int i = (graph_->box_x1 - graph_->box_x0)/omega + 1;//how many windows in x in the box
    int j = (graph_->box_y1 - graph_->box_y0)/omega + 1;//how many windows in y in the box
    if(_index2 % i == i - 1){
    	x1 = graph_->box_x1;
    	x0 = x1 - omega;
    }else{
		x0 =  (_index2 % i) * omega + graph_->box_x0;
		x1 = x0 + omega;
	}

	if(_index1 / i == j - 1){
		y1 = graph_->box_y1;
		y0 = y1 - omega;
	}else{
		y0 = _index1 / i + graph_->box_y0;
		y1 = y0 + omega;
	}
	*/
	if(a->_y1 >= _y1 && a->_y0 <= _y0){
		if(a->_x1 >= _x1){
			if(a->_x0 <= _x0){
				return (_x1 - _x0)*(_y1 - _y0);
			}
			else {
				return (_x1 - a->_x0)*(_y1 - _y0);
			}
		}else{
			if(a->_x0 <= _x0){
				return (a->_x1 - _x0)*(_y1 - _y0);
			}else{
				return(a->_x1 - a->_x0)*(_y1 - _y0);
			}
		}
	}else if(a->_x1 >= _x1 && a->_x0 <= _x0){
		if(a->_y1 >= _y1){
			if(a->_y0 <= _y0){
				return (_x1 - _x0)*(_y1 - _y0);
			}else{
				return (_x1 - _x0)*(_y1 - a->_y0);
			}
		}else{
			if(a->_y0 <= _y0){
				return (_x1 - _x0)*(a->_y1 - _y0);
			}else{
				return (_x1 - _x0)*(a->_y1 - a->_y0);
			}
		}
	}

	if(a->_y1 >= _y1){
		if(a->_x1 >= _x1){
			return (_x1 - a->_x0)*(_y1 - a->_y0);
		}else if(a->_x0 <= _x0){
			return (a->_x1 - _x0)*(_y1 - a->_y0);
		}else{
			return (_y1 - a->_y0)*(a->_x1 - a->_x0);
		}
	}else if(a->_y0 <= _y0){
		if(a->_x1 >= _x1){
			return (a->_y1 - _y0)*(_x1 - a->_x0);
		}else if(a->_x0 <= _x0){
			return (a->_y1 - _y0)*(a->_x1 - _x0);
		}else{
			return (a->_y1 - _y0)*(a->_x1 - a->_x0);
		}
	}else {
		if(a->_x1 >= _x1){
			return (a->_y1 - a->_y0)*(_x1 - a->_x0);
		}else if(a->_x0 <= _x0){
			return (a->_y1 - a->_y0)*(a->_x1 - _x0);
		}else{
			return (a->_y1 - a->_y0)*(a->_x1 - a->_x0);
		}
	}/*
	int countx1, countx2, county1, county2;
	countx1 = (a->_x0 > x0)? a->_x0: x0;
	countx2 = (a->_x1 < x1)? a->_x1: x1;
	county1 = (a->_y0 > y0)? a->_y0: y0;
	county2 = (a->_y1 < y1)? a->_y1: y1;

	return (countx2 - countx1)*(county2 - county1);*/
}

float Window::calden(){
	//vector<Shape*> member;
	vector<Shape*>::iterator it = member.begin();
	
	int omega = graph_->omega;
	int color1=0, color2=0;
	while(it!= member.end()){
		if((*it)->color == 1){
			color1 += abs(area(*it));
		}else if((*it)->color == 2){
			color2 += abs(area(*it));
		}

		it++;
	}

	cout<<"color1 " <<color1<<endl;
	cout<<"color2 "<<color2<<endl;


	_density1 = 100*(float)color1 / (omega*omega);
	_density2 = 100*(float)color2 / (omega*omega);
	_difference = _density1 - _density2;
	_difference = (_difference > 0)? _difference: -_difference;
	return _difference;	
}
