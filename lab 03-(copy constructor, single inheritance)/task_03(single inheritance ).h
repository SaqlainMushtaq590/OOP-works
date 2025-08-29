#include<iostream>
#include<string>

using namespace std;

class Person{
	protected:
	
	string name;
	int age;
	
	public:
		
		setPerson(string n, int a){
			name= n;
			age = a;
		}
		
		
		getPerson(){
			cout<<" name is "<<name<<endl;
			cout<<" age  is "<<age <<endl;
		}
};



class Student:public Person{
	protected:
	
	int rollNo;
	
public:
	
	setStudent(string n, int a, int r){
	setPerson(n, a);
		
		rollNo=r;
	}
	
	getStudent(){
		getPerson();
		cout<<"Roll no is "<<rollNo<<endl;
	}


};
