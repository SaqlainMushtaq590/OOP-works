#include<iostream>
#include<string>
using namespace std;

class Employee{
		int id;
		string name;
		float salary;
		
	public:
		
	Employee(){
		id=0;
		name="Not Assigned";
		salary=0.0;
				
		}
		
	displayDetails(){
		
		cout<<" Name : "<<name<<"\n id : "<<id<<"\n salary : "<<salary<<endl;	
	
	}
	
};
