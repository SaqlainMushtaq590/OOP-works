#include<iostream>
#include<string>

using namespace std;
 
class Shape{
	public:
	
	Shape(){
cout<<"Shape constructor called "<<endl;
	}
};

class Rectangle: public Shape{
	public:
	
	Rectangle(){
cout<<"Rectangle constructor called "<<endl;
	}
};
