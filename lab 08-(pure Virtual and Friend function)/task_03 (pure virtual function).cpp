#include<iostream>
using namespace std;

class shape{
	
	public:
	virtual double area()=0;
};

class circle:public shape{
	public:
		
		double radius;
		
		 circle(int r) {
		 	radius=r;
		 	
		 }
       double area(){
	   
	    cout<<"Area of Circle = "<<3.147*radius*radius<<endl;
	   }
	
};

class rectangle: public shape{
	public:
		
		double length, width;
		
		rectangle(int l, int w){
			length=l;
			width=w;
			
		}
		
		double area(){
		
		cout<<"Area of Rectangle = "<<length*width;
		 
		}
};

int main()
{
	circle  c(5);
	rectangle r(4,6);
	
	c.area();
	r.area();
	
}
