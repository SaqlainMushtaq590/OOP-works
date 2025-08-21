#include<iostream>
#include<string>
using namespace std;
class Rectangle{
	public:
	int length;
	int width;
	void setDimensions();
	void area();
	void perimeter();
	
};
void Rectangle::setDimensions(){
	cout<<"\nEnter the length of rectangle : ";
	cin>>length;
	cout<<"\nEnter the width of rectangle : ";
	cin>>width;
}
void Rectangle::area(){
   int area=length*width;
   cout<<"\n Area of rectangle = "<<area<<endl;
}
void Rectangle::perimeter(){
   float perimeter=2*(length+width);
   cout<<"\n Perimeter of rectangle  = "<<perimeter;
}

