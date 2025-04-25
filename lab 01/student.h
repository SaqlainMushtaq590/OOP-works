#include<iostream>
#include<string>
using namespace std;

class Student{
	public:
	// Data Members
	string name;
	int roll_Number;
	float marks;
	
	// Member Functions
	
	void getData(){         //Input Function
		cout<<"Enter the name  : ";
		cin>>name;
		cout<<"\nEnter the roll  : ";
		cin>>roll_Number;
		cout<<"\nEnter the marks  : ";
		cin>>marks;
	}
	
	void displayData(){     //Output Function
		cout<<"\n Name   : "<<name<<endl;
		cout<<"\n Roll   : "<<roll_Number<<endl;
		cout<<"\n Marks  : "<<marks<<endl;
	}
};
