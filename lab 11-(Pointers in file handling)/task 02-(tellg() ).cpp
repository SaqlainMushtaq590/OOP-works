#include<iostream>
#include<fstream>
using namespace std;
int main(){
	fstream in("greet.txt", ios::in);
	cout<<"Position of read pointer before reading: "<< in.tellg() <<endl;
	char ch;
	in.get(ch);
	cout << "Position of read pointer after reading a single character: "<<in.tellg() << endl;
	in.close();
	return 0;
}
