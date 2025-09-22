#include<iostream>
#include<fstream>
using namespace std;
int main(){
	fstream out("greet.txt", ios::out);
	cout<<"Position write pointer before writing: " << out.tellp() << endl;
	out<<"A";
	cout<<"Position of write pointer after writing a single character: "<<out.tellp()<<endl;
	out.close();
	return 0;
}
