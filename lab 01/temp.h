#include<iostream>
#include<string>
using namespace std;


class Temprature{
	public:
		float celcius;
		
		double convert_temp(){
			 double Farenheight=((celcius*9)/5)+32;
			 return Farenheight;
		}
	    void display_value(){
	    	cout<<"\n The temprature in Farenheight is = "<<convert_temp();
		}
};
