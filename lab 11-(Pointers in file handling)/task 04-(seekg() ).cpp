#include<iostream>
#include<fstream>
using namespace std;
int main(){
	fstream file ("student.txt", ios::in | ios::out);
	cout << "Position of pointer before reading: " << file.tellg() << endl;
	char ch;
	cout << "Move the read pointer position to 13" << endl;
	file.seekg(13);
	while(file.get(ch)){
		cout << "Character Read: " << ch << "Position of pointer after reading: " << file.tellg() << endl;
		
	}
	file.close();
	return 0;
}
