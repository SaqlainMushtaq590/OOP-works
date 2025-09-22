#include<iostream>
#include<fstream>
using namespace std;
int main(){
	fstream file("student.txt", ios:: in | ios::out |ios::trunc);
	file << "01, Awais, 3.5";
	file << "02, Saqlain, 3";
	file << "03, Zahid, 2.5";
	file << "04, Ahemd, 4.0";
	cout << "Position of write pointer after writing : " <<file.tellp() << endl;
	
	file.seekp(-13, ios::cur);
	file << "05, Nadir, 2.5";
	file.close();
	return 0;
}
