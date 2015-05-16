#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
using namespace std;

int main(int argc, char **argv)
{
	fstream fin(argv[1]);
	
	char buf[1024];
	char c;
	int alpha;
	int beta;
	int omega;
	int parameter[3];
	int node_count=0;

	if(fin.is_open()) {
		for(int i=0; i<3; i++) {
			fin.getline(buf,sizeof(buf));
			int pos = 0;
			while(buf[pos] != '=') { //cout << buf[pos];
				pos++;
			}//cout << endl;
			pos++;
			int num_char=0;
			while(isalnum(buf[pos])) { //cout << buf[pos];
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

		cout << "ALPHA=" << alpha <<endl;
		cout << "BETA=" << beta << endl;
		cout << "OMEGA=" << omega << endl;

		int index=0;
		int coordinate[4]={0,0,0,0};
		while(!fin.eof()) {
			node_count = 0;
			// Shape (index, x0, y0, x1, y1)
			cout << "#" << index << ":" ;
			cout << "(" << coordinate[0] << "," << coordinate[1] << ")   (" 
			     << coordinate[2] << "," << coordinate[3] << ")" << endl;
			while(node_count < 4) {
				fin.get(c);
				int num_char=0;
				while(isalnum(c)) { //cout << c << " ";
					int temp = c - '0';
					num_char *= 10;
					num_char += temp;
					fin.get(c);
				}		
				coordinate[node_count] = num_char;
				node_count++;
			}
			index++;
		}
	}
	else cout << "Fail to open file" << endl;
} 
