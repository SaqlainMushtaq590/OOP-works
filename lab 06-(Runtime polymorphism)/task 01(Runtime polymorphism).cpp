#include<iostream>
using namespace std;

class Shape{
	public:
		
	virtual double area(){
		
	}
};

class Circle:public Shape{
	public:
		int radius;
		
		Circle(int r){
			radius = r;
		}
		
		double area()override{
			
			cout<<"Area of Circle is = "<<3.147*radius*radius<<endl;
			
		}
};

class Rectangle: public Shape{
	public:
		int length;
		int width;
		
		Rectangle(int l, int w){
			
			length = l;
			width = w;
		}
		
		double area()override{
		
		cout<<"Area of Rectangle is = "<<length*width<<endl;
		}
};


int main(){
  Shape* shape;

    // Rectangle object
    Rectangle rect(10, 5);
    shape = &rect;
    shape->area();

    // Circle object
    Circle circ(7);
    shape = &circ;
    shape->area();

    return 0;

}
