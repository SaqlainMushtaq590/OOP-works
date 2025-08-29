#include<iostream>
using namespace std;
  
  class Employee{
  	public:
  		string name;
  		double salary;
  		
  		
  	void displayEmployee(){
  		cout<<"Name                : "<<name<<endl;
  		cout<<"Salary              : "<<salary<<endl;
	  }
  };
  
  
  
  class Developer:virtual public Employee{
  	public:
  		string programming_language;
  		
  		
  	void display_developer(){
  		displayEmployee();
  		cout<<"Language            : "<<programming_language<<endl;
	  }
  };
  
  
  class Designer: virtual public Employee{
  	public:
  		string design_tool;
  		
  		
  	void display_designer(){
  		displayEmployee();
  		cout<<"Tool                : "<<design_tool<<endl;
	  }
  };
  
  
  int main(){
  	Designer d1;
  	d1.name="ali";
  	d1.salary=25000;
  	d1.design_tool= "Alastrator";
  	d1.display_designer();
  	
  	cout<<"\n\n\n";
  	
  	Developer d2;
  	d2.name="deen";
  	d2.salary=45000;
  	d2.programming_language="C++, Java, Python, HTML, CSS, PHP";
  	d2.display_developer();
  }
