#include<iostream>
#include<fstream>
#include<string>

using namespace std;
//===================================================task 1(create file, write lines in it and append it)=========================================
int main(){
     fstream fileout("note.txt" , ios::out);
     fileout<<"Assalam o alaikum"<<endl;
     fileout<<"Welcome to OOP class"<<endl;
     fileout<<"This is all about File Handling System"<<endl;
     fileout.close();

     fstream file_a("note.txt" , ios::app);
     file_a<<"Myself Saqlian Mushtaq"<<endl;
     file_a<<"Roll no: 2024-SE-35";
     file_a.close();

     fstream filein("note.txt" , ios::in);
     string s;
     while(!filein.eof()){
     	
        getline(filein,s);
	cout<<s<<endl;
     }
     filein.close();
//================================================task 2(count lines in the file)===================================================================
     
     fstream file_c("note.txt" , ios::in);
     string line;
     int linecount=0;
     
     while(getline(file_c , line)){
     	linecount++;
     	
	 }
	cout<<"number of lines: "<<linecount<<endl;
	file_c.close();
	
//===============================================task 3(copy content in another file)================================================================
	string copy;
	fstream original("note.txt" , ios::in);
	fstream copyfile("note_copy.txt" , ios::out);
	while(getline(original , copy)){
		copyfile<<copy<<endl;
	}
	cout<<"\ncode copyed successfully!\n"<<endl;
	original.close();
	copyfile.close();
	
	
	
	cout<<"<- copyed code ->"<<endl;
	fstream file_show("note_copy.txt", ios::in);
	while(getline(file_show, copy)){
		cout<<copy<<endl;
	}
}
