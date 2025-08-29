#include<iostream>
#include<string>

using namespace std;

class Rectangle{
	 float length, width;
	 
public:
	Rectangle(){
		length =1.0;
		width =1.0;
		
	}
	
	Rectangle(float a, float b){
		length = a;
		width = b;
	}
	
	Rectangle(float x){
		length=x;
		width=x;
	}
	
	area(){
		cout<<"Area of Rectngle is = "<<length*width<<endl;
		cout<<"********************"<<endl;
	}
	
	display(){
		cout<<"Length = "<<length<<endl;
		cout<<"Width = "<<width<<endl;
	}
};
