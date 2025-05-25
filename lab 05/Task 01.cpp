#include<iostream>
using namespace std;

class Person{
	public:
	string name;
	int    age;
	

	void display_personInfo(){
	
	cout<<"Name : "<<name<<endl;
	cout<<"Age  : "<<age <<endl;
}
};

class Student:public Person{
	public:
	
	int student_id;

	void display_student_id(){
	display_personInfo();
	cout<<"ID : "<<student_id;
}
	
};


int main(){
	Student s1;
	s1.name="ali";
	s1.age=12;
	s1.student_id=35;
    s1.display_student_id();

}
