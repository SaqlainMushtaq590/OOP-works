#include<iostream>
#include<fstream>
using namespace std;

int main()
{
	fstream filestd("Students.txt",ios::out);
	filestd<<" Name                 Father name         Roll no   "<<endl;
	filestd<<"===================================================="<<endl;
	filestd<<" Saqlain Mushtaq      Jaffar Ali         2024-SE-35 "<<endl;
	filestd<<" Jawahir Ali          Mehboob Ali        2024-SE-34 "<<endl;
	filestd<<" Kamal Ali            Mod Sadiq          2024-SE-38 "<<endl;
	filestd.close();
	
	fstream filein("Students.txt" , ios::in);
	string s;
	while(getline(filein,s))
	{
		cout<<s<<endl;
	}
	filein.close();
}
